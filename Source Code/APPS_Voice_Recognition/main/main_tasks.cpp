/**
 * @file main_tasks.cpp
 * @author Jan Sevrin
 * @brief Contains the RTOS tasks used to process speech quality.
 * @version 0.1
 * @date 2022-06-27
 * 
 * @copyright Fully open-source
 * 
 */

//Standard library
#include <vector>
#include <deque>
#include <string>

//ESP-IDF includes
#include "esp_log.h"
#include "esp_system.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

//Custom includes
#include "BLE.hpp"
#include "custom_dsp.hpp"
#include "microphone_i2s.hpp"

//Main header
#include "main_tasks.hpp"

static constexpr bool is_powerof2(size_t size) {
    return size && ((size & (size - 1)) == 0);
}

enum class AudioStatus {WAIT, VEC1_READY, VEC2_READY};
AudioStatus audio_status = AudioStatus::WAIT;

//read_audio() globals
Microphone inmp441(I2S_NUM_0, 26, 25, 33);
constexpr size_t SAMPLES_LENGTH = Microphone::buffer_size * 4;
std::vector<float> audio_vec1(SAMPLES_LENGTH);
std::vector<float> audio_vec2(SAMPLES_LENGTH);
static_assert(SAMPLES_LENGTH, "SAMPLES_LENGTH, which determines vector size, must be a power of 2!");

/**
 * @brief Convert standard deviation to a string value representing voice quality.
 *
 * @param standard_deviation Input.
 * @return std::string Output. First character of the string is a value from 0 to 255.
 */
static std::string get_quality_value(const float standard_deviation)
{
	char quality_value;
	if(standard_deviation <= 10.0f)
	{
		quality_value = 1;
	}
	else if(standard_deviation >= 20.0f)
	{
		quality_value = 255;
	}
	else
	{
		//255(char max) - 2(values already used) = 253 values left.
		//20.0(max SDNN) - 10.0(min SDNN) = 10.0f left.
		constexpr float factor = 253 / 10.0f;
		quality_value = (standard_deviation - 10.0f) * factor + 1;
	}
	return std::string(1, quality_value);
}

/**
 * @brief Process voice quality into a value and attempt to send it via BLE.
 *
 * @param audio Input. Will be mutated.
 */
static void process_voice_quality(std::vector<float>& audio)
{
	static constexpr size_t FREQ_PEAKS_MAX_LENGTH = 1.0f * Microphone::sample_rate / SAMPLES_LENGTH * 2; //2 seconds
	static constexpr unsigned int min_fundamental_frequency = 50;
	static constexpr unsigned int max_fundamental_frequency = 175;
	static std::deque<float> frequency_peaks;
	static dsp::signal_point peak;

	peak = dsp::get_voice_peak(
						audio,
						min_fundamental_frequency,
						max_fundamental_frequency,
						Microphone::sample_rate);

	if(peak.amplitude > 0.15f)
	{
		//ESP_LOGI("BLE", "peak: %d Hz", peak.frequency);
		frequency_peaks.push_front(peak.frequency);
		if(frequency_peaks.size() > FREQ_PEAKS_MAX_LENGTH)
		{
			frequency_peaks.pop_back();
		}

		if(frequency_peaks.size() == FREQ_PEAKS_MAX_LENGTH)
		{
			const float sd = dsp::get_standard_deviation(frequency_peaks);
			//ESP_LOGI("BLE", "sd: %f", sd);
			std::string quality_value = get_quality_value(sd);
			//ESP_LOGI("BLE", "Sending value: %d", quality_value[0]);
			try_send_voice_values("0", quality_value);
		}
	}
}

/**
 * @brief Constantly update global vectors with audio data.
 *
 * @param params N/A.
 */
void task_read_audio(void* params)
{
	while(true)
	{
		if(inmp441.read_full(audio_vec1) == ESP_OK)
		{
			audio_status = AudioStatus::VEC1_READY;
		}

		if(inmp441.read_full(audio_vec2) == ESP_OK)
		{
			audio_status = AudioStatus::VEC2_READY;
		}
	}
}

/**
 * @brief Constantly process the most recently updated audio vector.
 *
 * @param params N/A.
 */
void task_process_audio(void* params)
{
	while(true)
	{
		switch(audio_status)
		{
			case AudioStatus::VEC1_READY:
				audio_status = AudioStatus::WAIT;
				process_voice_quality(audio_vec1);
				break;
			case AudioStatus::VEC2_READY:
				audio_status = AudioStatus::WAIT;
				process_voice_quality(audio_vec2);
				break;
			default:
				vTaskDelay(10 / portTICK_PERIOD_MS); //Wait 10ms until audio is ready for processing.
				break;
		}

	}
}




