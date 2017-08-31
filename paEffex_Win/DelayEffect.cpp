#include "DelayEffect.h"
#include "MsgStruct.h"
#include <cmath>
#include <iostream>
using namespace std;

DelayEffect::DelayEffect() {
	
	
	dlyGain_ = 0.5; // default gain
	dlyTime_ = 400.0; // default delay
	sampleRate_ = 44100;  // default sample rate
	sample_cntr_ = 0;	  // buffer counter
	dlySamples_ = floor((dlyTime_ / 1000) * sampleRate_); // Number of samples in delay time
	dlyBufferSize_ = 2 * sampleRate_;
	
	dlyBuffer_ = new int[88200]; // Internal Delay buffer
		
	// Initialize buffer
	for (int i = 0; i < dlyBufferSize_; i++)
	{
		dlyBuffer_[i] = 0; 
	}

	//cout << " -- Delay effect created -- " << endl;
}

//TODO
/*
Investigate:
- Improve barrier to check that userdata is of type MsgFromUser
- Feedback if error while changing parameter values (try/catch)

*/
int DelayEffect::changeEffectParameters(void * userdata) {
	
	effectParamsChanged_ = false;

	MsgFromUser* msg = (MsgFromUser*)userdata;
	if (!(msg->header_.EffectName == "delay")){
		return 0;
	}
	
	// # PARAM 1 # Delay time. 
	if (!(msg->body_.params[0] > 0 && msg->body_.params[0] < dlyBufferSize_ / sampleRate_ * 1000)) {
		dlyTime_ = (int) msg->body_.params[0];
		dlySamples_ = floor((dlyTime_ / 1000) * sampleRate_);
		cout << "Effect parameters changed to: DlyTime: " << dlyTime_ << endl;
	}
	else if(msg->body_.params[0] < 0) {
		dlyTime_ = 0;
	}
	else if (msg->body_.params[0] > dlyBufferSize_ / sampleRate_ * 1000) {
		dlyTime_ = dlyBufferSize_ / sampleRate_ * 1000;
	}
	
	// # PARAM 2 # Delay gain. 
	if (msg->body_.params[1] >= 0.0 && msg->body_.params[1] < 1.0)
	{
		dlyGain_ = msg->body_.params[1];
		cout << "Effect parameters changed to: Gain:  " << dlyGain_ << endl;
	}

	// Report effect changed
	effectParamsChanged_ = true;

	return 0;
}


int DelayEffect::applyEffect(int * iData, int* oData, int bufferSize)
{	
	// Skip effect if disabled
	//if (!enableEffect_) return 0;
	
	// Process data
	// Add delay out(n) = in(n) + gain*out(n-m), where m = delaytime in samples
	// If n-m < 0, go to end of buffer to find delay sample
	
	int dlypos; // position of delayed signal in delay buffer	
	int currpos; // real time position in delay buffer
	int sample;
	int delaysignal;
	int j = 0;
	
	for (int i = 0; i < bufferSize; i++){
		
		currpos = bufferSize*sample_cntr_ + i;
		dlypos= currpos - dlySamples_;
		if (dlypos< 0){			
			// Index less than zero, go to end of buffer to find older samples.
			dlypos= dlyBufferSize_ + currpos - dlySamples_;
		}

		// Update delay buffer. Load oData to delaybuffer to replace the oldest data in buffer.		
		delaysignal = (int)(dlyGain_*dlyBuffer_[dlypos]);
		oData[j]   = iData[j]   + delaysignal;
		oData[j+1] = iData[j+1] + delaysignal;
		dlyBuffer_[currpos] = oData[j];
		
		j += 2;		
	}
	
	//Increment buffer counter
	sample_cntr_ += 1;
	
	//Reset buffer counter
	if (sample_cntr_ >= dlyBufferSize_ / bufferSize){
		sample_cntr_ = 0;
	}	
	
	return 0;
}


void DelayEffect::set_gain_(double gain)
{
	dlyGain_ = gain;
}

void DelayEffect::set_dlyTime_(double dlyTime)
{
	dlyTime_ = dlyTime;
}

int DelayEffect::get_gain_()
{
	return dlyGain_;
}
int DelayEffect::get_dlyTime_()
{
	return dlyTime_;
}

DelayEffect::~DelayEffect()
{
	delete dlyBuffer_;
}