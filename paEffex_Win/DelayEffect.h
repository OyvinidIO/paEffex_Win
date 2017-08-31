#pragma once
#include "IGuitarEffect.h"
class DelayEffect :
	public IGuitarEffect
{
public:
	DelayEffect();
	~DelayEffect();
//	int applyEffect(double * iData, double * oData, int bufferSize);
	int applyEffect(int * iData, int* oData, int bufferSize);
	int changeEffectParameters(void * userdata);

	void set_gain_(double gain);
	void set_dlyTime_(double dlyTime);
	int get_gain_();
	int get_dlyTime_();

private:	
	int dlyBufferSize_;
	int * dlyBuffer_; // Buffer with samples for delay processing
	int sample_cntr_;
	double dlyGain_; // Delay gain
	double dlyTime_; // MS between each repeat
	int dlySamples_; // delay time in samples
	int sampleRate_;
};

