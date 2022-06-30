/**
 * @file custom_dsp.hpp
 * @author Jan Sevrin
 * @brief Custom digital signal processing module used for speech analysis.
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Fully open-source
 * 
 */

#pragma once

//Standard library
#include <limits>
#include <vector>
#include <deque>
#include <cmath>

namespace dsp {
	/**
	 * @brief Structure to hold a signal's frequency and its amplitude.
	 * 
	 */
	struct signal_point {
		int frequency;
		float amplitude;
	};

	/**
	 * @brief Remove DC-offset.
	 * 
	 * @param vec Input and output.
	 */
	void remove_DC(std::vector<float>& vec);
	/**
	 * @brief Apply hamming window.
	 * 
	 * @param vec Input and output.
	 */
	void hamming_window(std::vector<float>& vec);
	/**
	 * @brief Remove phase-offsets of sinusoids and apply logarithm on their amplitudes on a DTFT.
	 * 
	 * @param vec Input and output. Should be result of dsp::rfft() or have the same format.
	 */
	void custom_logarithm_abs(std::vector<float>& vec);
	/**
	 * @brief Get signal peak quefrequency(fundamental frequency).
	 * 
	 * @param audio_data Input.
	 * @param frequency_floor Minimum quefrequency.
	 * @param frequency_ceiling Maximum quefrequency.
	 * @param sample_rate Used to convert index position to frequency.
	 * @return signal_point Output. Frequency and ampltude of peak.
	 */
	signal_point get_voice_peak(
		std::vector<float> audio_data,
		const unsigned int& frequency_floor,
		const unsigned int& frequency_ceiling,
		const int& sample_rate);
	/**
	 * @brief Get signal peak within certain vector indexes.
	 * 
	 * @param cepstrum Input.
	 * @param floor Minimum index.
	 * @param ceiling Maximum index.
	 * @param sample_rate Used to convert index position to frequency.
	 * @return signal_point Output. Frequency and ampltude of peak.
	 */
	signal_point get_peak(const std::vector<float>& cepstrum, const size_t& floor, const size_t& ceiling, const int& sample_rate);
	/**
	 * @brief Get standard deviation.
	 * 
	 * @param values Input.
	 * @return float Output. Standard deviation.
	 */
	float get_standard_deviation(const std::deque<float>& values);
	/**
	 * @brief Compute RFFT of a vector and store it in-place in the same vector.
	 * 
	 * @param vec Input and output.
	 */
	void rfft(std::vector<float>& vec);
	/**
	 * @brief Compute IRFFT of a vector and store it in-place in the same vector.
	 * 
	 * @param vec Input and output.
	 */
	void irfft(std::vector<float>& vec);

}
