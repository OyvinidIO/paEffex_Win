#pragma once
#include "IGuitarEffect.h"
#include <vector>
#include <fstream>


class EffectManager
{
public:	
	EffectManager();
	void setEffect(IGuitarEffect* effect);
	//int applyEffect(const double* iData, double* oData, int bufferSize);
	int applyEffect(const int* iData, int* oData, int bufferSize);
	void changeEffectParameters(void * userdata);
	void deallocateEffects();
	~EffectManager();

	bool effectParamsChanged_;

private:
	std::vector<IGuitarEffect*> effects_;
	bool streamStarted_;
	std::ofstream inputdata_, outputdata_; // Logging streams
};

