#include "EffectManager.h"
#include <iostream>

using namespace std;

EffectManager::EffectManager() {
	inputdata_.exceptions(ofstream::failbit);
	outputdata_.exceptions(ofstream::failbit);
	streamStarted_ = false;
	effectParamsChanged_ = false;
}

void EffectManager::setEffect(IGuitarEffect* effect) {
	effects_.emplace_back(effect);
}

int EffectManager::applyEffect(const void* iData, void* oData, unsigned long bufferSize) {
	
	const float* inputData = (const float*)iData;
	float* outputData      = (float*)oData;
	unsigned long i = 0;
	for ( i = 0; i < 2*bufferSize; i+=2) {
		outputData[i]   = inputData[i];
		outputData[i+1] = inputData[i+1];
		//outputData[i+2] = inputData[i+2];
		//outputData[i+3] = inputData[i+3];
		//outputData[i+4] = inputData[i+4];
		//outputData[i+5] = inputData[i+5];
		//outputData[i+6] = inputData[i+6];
		//outputData[i+7] = inputData[i+7];
		
		/**outputData++   = *inputData++;
		*outputData++   = *inputData++;*/
		
	}
	//cout << "BUFFERSIZE: " << bufferSize << ", i: " << i << endl;
	// Apply effects
	for (auto& element : effects_) element->applyEffect(outputData, outputData, bufferSize);
	
	//// Log I/O
	//if (!streamStarted_) {
	//	streamStarted_ = true;
	//	cout << "Opening file for recording..." << endl;
	//	try {			
	//		inputdata_.open("Inputstream.txt");
	//		outputdata_.open("Outputstream.txt");
	//	}
	//	catch (fstream::failure &open_error) {
	//		cerr << "\n\n Exception occured when opening a file\n" << open_error.what() << endl;
	//		return 0;
	//	}
	//}

	//if (inputdata_.is_open() && outputdata_.is_open()) {
	//	for (unsigned int i = 0; i < bufferSize; i += 30) {
	//		inputdata_ << iData[i]  << endl;
	//		outputdata_ << oData[i] << endl;
	//	}
	//}
	//else {
	//	cout << "Error opening file" << endl;
	//}
	return 0;
}

void EffectManager::changeEffectParameters(void * userdata) {
	
	for (auto& element : effects_) {
		
		element->changeEffectParameters(userdata);
		effectParamsChanged_ = element->effectParamsChanged_; // Flag set when effect has returned change successful
		if (effectParamsChanged_) break;
	}
}

void EffectManager::deallocateEffects() {
	for (auto element : effects_)
		delete element;
	effects_.clear();
}
EffectManager::~EffectManager() {
	for (auto element : effects_)
		delete element;

	/*cout << "Closing files after recording" << endl;
	inputdata_.close();
	outputdata_.close();*/
}
