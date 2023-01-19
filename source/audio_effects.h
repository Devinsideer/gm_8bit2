#pragma once
#include <cassert>
#include <cstdint>
#include <cstring>

namespace AudioEffects {
	enum {
		EFF_NONE,
		EFF_BITCRUSH,
		EFF_DESAMPLE
	};

	void BitCrush(uint16_t* sampleBuffer, int samples, float quant, float gainFactor) {
		for (int i = 0; i < samples; i++) {
			// apply a low pass filter to remove high frequency noise
			sampleBuffer[i] = (sampleBuffer[i] + sampleBuffer[i-1]) / 2;
			// apply a distortion effect to emulate static noise
			sampleBuffer[i] = sampleBuffer[i] * 1.5;
			
			// apply a high pass filter to remove low frequency noise
			sampleBuffer[i] = sampleBuffer[i] - (sampleBuffer[i-1] / 2);
			
			// limit the amplitude to prevent clipping
			if (sampleBuffer[i] > 32767) {
				sampleBuffer[i] = 32767;
			} else if (sampleBuffer[i] < -32767) {
				sampleBuffer[i] = -32767;
			}
		}
	}

	static uint16_t tempBuf[10 * 1024];
	void Desample(uint16_t* inBuffer, int& samples, int desampleRate = 2) {
		assert(samples / desampleRate + 1 <= sizeof(tempBuf));
		int outIdx = 0;
		for (int i = 0; i < samples; i++) {
			if (i % desampleRate == 0) continue;

			tempBuf[outIdx] = inBuffer[i];
			outIdx++;
		}
		std::memcpy(inBuffer, tempBuf, outIdx * 2);
		samples = outIdx;
	}
}
