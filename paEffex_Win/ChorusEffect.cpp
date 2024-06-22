#include <cmath>
#include "ChorusEffect.h"

ChorusEffect::ChorusEffect()
{

	ChorusEffect::frequency = 10;
	ChorusEffect::oscillator = 1;
	ChorusEffect::oscillatortime = 0;

}

ChorusEffect::~ChorusEffect()
{
}

int ChorusEffect::applyEffect(float* iData, float* oData, unsigned long bufferSize)
{

	unsigned long channels = 2;
	for (unsigned long i = 0; i < channels * bufferSize; i += channels)
	{
		oscillator = std::sinf(2.0f * 3.14159f * frequency * oscillatortime) / 3.0;
		oscillatortime += 1.0f / 44100.0f;
		if (oscillatortime >= 44100.0f)
			oscillatortime = 0;

		oData[i] = iData[i] + iData[i] * oscillator;
		oData[i + 1] = iData[i + 1] + iData[i + 1] * oscillator;
	}

	return 0;
}

int ChorusEffect::changeEffectParameters(void* userdata)
{
	return 0;
}
