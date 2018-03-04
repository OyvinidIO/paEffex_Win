#pragma once
#include "IGuitarEffect.h"
class DelayEffect :
	public IGuitarEffect
{
public:
	DelayEffect();
	~DelayEffect();
//	int applyEffect(double * iData, double * oData, int bufferSize);
	int applyEffect(float * iData, float* oData, unsigned long bufferSize);
	int changeEffectParameters(void * userdata);

	void set_gain_(double gain);
	void set_dlyTime_(double dlyTime);
	unsigned long get_gain_();
	unsigned long get_dlyTime_();

private:	
	unsigned long dlyBufferSize_;
	float * dlyBuffer_; // Buffer with samples for delay processing
	unsigned long sample_cntr_;
	float dlyGain_; // Delay gain
	float dlyTime_; // MS between each repeat
	unsigned long dlySamples_; // delay time in samples
	unsigned long sampleRate_;
};

