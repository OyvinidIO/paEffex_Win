#pragma once

class IGuitarEffect
{
public:
	IGuitarEffect();
//	virtual int applyEffect(double* iData, double* oData, int bufferSize) = 0;
	virtual int applyEffect(float* iData, float* oData, unsigned long bufferSize) = 0;
	virtual int changeEffectParameters(void * userdata) = 0;
	virtual ~IGuitarEffect();
	bool enableEffect_;
	bool effectParamsChanged_;
};

