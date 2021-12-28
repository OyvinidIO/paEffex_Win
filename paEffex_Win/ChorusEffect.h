#pragma once
#include "IGuitarEffect.h"
class ChorusEffect : public IGuitarEffect
{
	public:
		ChorusEffect();
		~ChorusEffect();
		int applyEffect(float* iData, float* oData, unsigned long bufferSize);
		int changeEffectParameters(void* userdata);

private:
	float frequency;
	float oscillator;
	float oscillatortime;

};

