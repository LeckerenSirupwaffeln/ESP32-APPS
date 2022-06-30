/**
 * @file custom_dsp.cpp
 * @author Jan Sevrin, Takuya Ooura(partly)
 * @brief Custom digital signal processing module used for speech analysis.
 * @version 0.1
 * @date 2022-04-21
 * 
 * @copyright Fully open-source
 * 
 */

//Standard library
#include <limits>
#include <vector>
#include <deque>
#include <cmath>
#include <algorithm>
#include <numeric>

//ESP-IDF library
#include "esp_log.h"

//Main header
#include "custom_dsp.hpp"

constexpr float PI = 3.1415927410125732421875f;

namespace dsp
{
	void remove_DC(std::vector<float>& vec)
	{
		const int N = vec.size();
		float avg = 0.0f;
		for(const auto& item : vec)
			avg += item;
		avg /= N;
		for(auto& item : vec)
			item -= avg;
	}

	void hamming_window(std::vector<float>& vec)
	{
		constexpr float PI = 3.14159265358979323846f;
		constexpr float TWO_PI = 2 * PI;
		const int N = vec.size();
		for(unsigned int i = 0; i < N; ++i)
		{
			const float coeffecient = 0.54 - 0.46 * std::cos(TWO_PI * i / N);
			vec[i] = vec[i] * coeffecient;
		}
	}

	void custom_logarithm_abs(std::vector<float>& vec)
	{
		constexpr float eps = std::numeric_limits<float>::epsilon();
		const unsigned int N = vec.size();
		const unsigned int half_N = vec.size() / 2;

		vec[0] = std::log(std::abs(vec[0]) + eps);
		for(unsigned int i = 1; i < half_N; ++i)
		{
			const float real = vec[i];
			const float imag = vec[N - i];
			const float abs = std::sqrt(real * real + imag * imag);
			vec[i] = std::log(abs + eps);
			vec[N - i] = 0.0f;
		}
		vec[half_N] = std::log(std::abs(vec[half_N]) + eps);
	}

	signal_point get_voice_peak(
		std::vector<float> audio_data,
		const unsigned int& frequency_min,
		const unsigned int& frequency_max,
		const int& sample_rate)
	{
		dsp::remove_DC(audio_data);
		dsp::hamming_window(audio_data);

		// Cepstrum = IFFT(log(abs(FFT(vector)))
		dsp::rfft(audio_data);
		dsp::custom_logarithm_abs(audio_data);
		dsp::irfft(audio_data);

		const size_t min_pos = sample_rate / frequency_max;
		const size_t max_pos = sample_rate / frequency_min;
		dsp::signal_point peak = dsp::get_peak(audio_data, min_pos, max_pos + 1, sample_rate);
		return peak;
	}

	signal_point get_peak(const std::vector<float>& cepstrum, const size_t& floor, const size_t& ceiling, const int& sample_rate)
	{
		const std::vector<float>::const_iterator itr_floor = cepstrum.begin() + floor;
		const std::vector<float>::const_iterator itr_ceiling = cepstrum.begin() + ceiling;
		const auto peak_itr = std::max_element(itr_floor, itr_ceiling,
			[](auto lhs, auto rhs)
			{
				return std::abs(lhs) < std::abs(rhs);
			});

		const size_t peak_pos = std::distance(cepstrum.begin(), peak_itr);
		signal_point peak;
		peak.frequency = sample_rate / peak_pos;
		peak.amplitude = std::abs(*peak_itr);
		return peak;

	}

	float get_standard_deviation(const std::deque<float>& values)
	{
		const auto N = values.size();
		std::vector<float> deviations(N);

		const float mean = std::accumulate(values.begin(), values.end(), 0.0f) / N;
		for(auto i = 0; i < N; ++i)
		{
			float value_deviation = std::pow((values[i] - mean), 2);
			deviations[i] = value_deviation;
		}

		const float variance = std::accumulate(deviations.begin(), deviations.end(), 0.0f) / (N - 1);
		const float deviation = std::sqrt(variance);
		return deviation;
	}

	/*  Written by Takuya Ooura: https://www.kurims.kyoto-u.ac.jp/~ooura/
		Output: Re[0 .. N / 2], Im[N / 2 + 1 .. N - 1] in case of EVEN input.
		First and last Re value is always paired to a constat imaginary number: 0.0i,
		thus these 0.0i imaginaries are missing from the output array to save space. */
	void rfft(std::vector<float>& vec)
	{
		const int N = vec.size();
		/* ---- scrambler ---- */
		for (int j = 1, i = 0; j < (N - 1); ++j) {
			for (int k = (N >> 1); k > (i ^= k); k >>= 1);
			if (j < i) {
				const float xr = vec[j];
				vec[j] = vec[i];
				vec[i] = xr;
			}
		}
		float theta = -2.0f * PI;  /* -2*pi */
		for (int m, mh = 1; (m = mh << 1) <= N; mh = m) {
			const int mq = mh >> 1;
			theta *= 0.5f;
			/* ---- real to real butterflies (W == 1) ---- */
			for (int jr = 0; jr < N; jr += m) {
				const int kr = jr + mh;
				const float xr = vec[kr];
				vec[kr] = vec[jr] - xr;
				vec[jr] += xr;
			}
			/* ---- complex to complex butterflies (W != 1) ---- */
			for (int i = 1; i < mq; ++i) {
				const float wr = std::cos(theta * i);
				const float wi = std::sin(theta * i);
				for (int j = 0; j < N; j += m) {
					const int jr = j + i;
					const int ji = j + mh - i;
					const int kr = j + mh + i;
					const int ki = j + m - i;
					const float xr = wr * vec[kr] + wi * vec[ki];
					const float xi = wr * vec[ki] - wi * vec[kr];
					vec[kr] = -vec[ji] + xi;
					vec[ki] = vec[ji] + xi;
					vec[ji] = vec[jr] - xr;
					vec[jr] = vec[jr] + xr;
				}
			}
			/* ---- real to complex butterflies are trivial ---- */
		}
	}

	/*  Written by Takuya Ooura: https://www.kurims.kyoto-u.ac.jp/~ooura/ */
	void irfft(std::vector<float>& vec)
	{
		const int N = vec.size();

		vec[0] *= 0.5;
		vec[N/2] *= 0.5;
		float theta = 2.0f * PI / N;  /* 2*pi/N */
		for (int mh, m = N; (mh = m >> 1) >= 1; m = mh) {
			const int mq = mh >> 1;
			/* ---- real to real butterflies (W == 1) ---- */
			for (int jr = 0; jr < N; jr += m) {
				const int kr = jr + mh;
				const float xr = vec[jr] - vec[kr];
				vec[jr] += vec[kr];
				vec[kr] = xr;
			}
			/* ---- complex to complex butterflies (W != 1) ---- */
			for (int i = 1; i < mq; ++i) {
				const float wr = std::cos(theta * i);
				const float wi = std::sin(theta * i);
				for (int j = 0; j < N; j += m) {
					const int jr = j + i;
					const int ji = j + mh - i;
					const int kr = j + mh + i;
					const int ki = j + m - i;
					const float xr = vec[jr] - vec[ji];
					const float xi = vec[ki] + vec[kr];
					vec[jr] = vec[jr] + vec[ji];
					vec[ji] = vec[ki] - vec[kr];
					vec[kr] = wr * xr + wi * xi;
					vec[ki] = wr * xi - wi * xr;
				}
			}
			/* ---- complex to real butterflies are trivial ---- */
			theta *= 2;
		}
		/* ---- unscrambler ---- */
		for (int j = 1, i = 0; j < N - 1; j++) {
			for (int k = (N >> 1); k > (i ^= k); k >>= 1);
			if (j < i) {
				const float xr = vec[j];
				vec[j] = vec[i];
				vec[i] = xr;
			}
		}

		const float scaling = 2.0f / N;
		for(int i = 0; i < N; ++i)
			vec[i] *= scaling;
	}

}


