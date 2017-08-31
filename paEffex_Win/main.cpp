#include "portaudio.h"
#include "pa_asio.h"
#include "EffectManager.h"
#include <iostream>
#include <string>

#include "DelayEffect.h"
//#include "MsgStruct.h"

using namespace std;

// Using a global object to hold the effects for simplicity.
EffectManager effectManager;


//MsgFromUser UDPmsg;

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (64)

int processAudioStreamCallback(const void *input, void *output, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void * userData) {

	//effectManager.applyEffect((const double*)input, (double*)output, (int)framesPerBuffer);
	effectManager.applyEffect((const int*)input, (int*)output, (int)framesPerBuffer);
	return 0;
}
int main(void);
int main(void) {
	

	/* Stream parameters */
	PaStream *stream;
	PaError err = paNoError;
	PaStreamParameters inputParameters, outputParameters;
	PaStreamFlags streamFlags = paNoFlag; /*paClipOff paDitherOff paNeverDropInput paPrimeOutputBuffersUsingStreamCallback paPlatformSpecificFlags*/
	PaHostApiInfo APIInfo;
	

	/* PA info */
	const PaDeviceInfo* inputInfo;
	const PaDeviceInfo* outputInfo;
	PaHostApiIndex ApiIndex;
	const PaHostApiInfo * HostApiInfo;
	
	/* Internal variables for main()*/
	string waitForUserInput;
	

	/* Initialize PortAudio */
	err = Pa_Initialize();
	if (err != paNoError) {
		cout << "PA Initialize FAILED" << endl;
		return 0;
	}			
	
	/* Initialize Stream Parameters */	
	inputParameters.device = Pa_GetDefaultInputDevice();
	outputParameters.device = Pa_GetDefaultOutputDevice();

	if (inputParameters.device == paNoDevice) {
		cout << "Error: No default input device." << endl;
		goto error;		
	}
	if (outputParameters.device == paNoDevice) {
		cout << "Error: No default output device." << endl;
		goto error;
	}	
	inputInfo = Pa_GetDeviceInfo(inputParameters.device);
	outputInfo = Pa_GetDeviceInfo(outputParameters.device);

	inputParameters.channelCount =   inputInfo->maxInputChannels;
	outputParameters.channelCount = outputInfo->maxOutputChannels;
	inputParameters.sampleFormat = paInt32;/* +paNonInterleaved;*/
	outputParameters.sampleFormat = paInt32; /*+paNonInterleaved;*/
	inputParameters.suggestedLatency = inputInfo->defaultLowInputLatency;
	outputParameters.suggestedLatency = outputInfo->defaultLowOutputLatency;
	inputParameters.hostApiSpecificStreamInfo  = NULL;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	/*-----------------------------------------------------------------------------*/
	///*DEBUG INFO*/
	//// Show I/O paramteres	
	//ApiIndex = Pa_GetDefaultHostApi();
	//HostApiInfo = Pa_GetHostApiInfo(ApiIndex);
	//cout << "INPUT parameters---------" << endl;
	//cout << "     input DevID:        " << inputParameters.device << endl;
	//cout << "     Name:               " << inputInfo->name << endl;
	//cout << "     LL:                 " << inputInfo->defaultLowInputLatency << endl;
	//cout << "     HL:                 " << inputInfo->defaultHighInputLatency << endl;
	//cout << "     max in ch:          " << inputInfo->maxInputChannels << endl;
	//cout << "OUTPUT parameters--------" << endl;
	//cout << "     output DevID:       " << outputParameters.device << endl;
	//cout << "     Name:               " << outputInfo->name << endl;
	//cout << "     LL:                 " << outputInfo->defaultLowOutputLatency << endl;
	//cout << "     HL:                 " << outputInfo->defaultHighOutputLatency << endl;
	//cout << "     max out ch:         " << outputInfo->maxOutputChannels << endl;
	//cout << "     default samplerate: " << outputInfo->defaultSampleRate << endl;
	//cout << "-------------------------" << endl;
	//cout << "   Default API index:    " << ApiIndex << endl;
	//cout << "   Default Host API info " << endl;
	//cout << "      Input device:      " << HostApiInfo->defaultInputDevice << endl;
	//cout << "      Output device:     " << HostApiInfo->defaultOutputDevice << endl;
	//cout << "      Device count:      " << HostApiInfo->deviceCount << endl;
	//cout << "      Name:              " << HostApiInfo->name << endl;
	//cout << "      Struct ver:        " << HostApiInfo->structVersion << endl;
	//cout << "      Type:              " << HostApiInfo->type << endl;
	/*----------------------------------------------------------------------------------*/
	
	/* Create Effects */
	effectManager.setEffect(new DelayEffect());

	/* Open an audio I/O stream. */
	cout << " -- Opening stream -- " << endl;
	err = Pa_OpenStream(&stream,									
						&inputParameters, &outputParameters, 
						SAMPLE_RATE, 
						FRAMES_PER_BUFFER,
						streamFlags, 
						processAudioStreamCallback, 
						NULL);
	if (err != paNoError) { 
		cout << "--Error : Open stream failed. Errorcode: " << err << " : " << Pa_GetErrorText(err) << endl; 
		goto error;
	}
	cout << " -- Opening stream successful -- " << endl;

	/* Start an audio I/O stream. */
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		cout << "--Error : Start stream failed. Errorcode: " << err << " : " << Pa_GetErrorText(err) << endl;
		goto error;
	}
	cout << " -- Starting stream successful -- " << endl;

	/*  -- Block thread until user hits enter -- */
	cout << endl << "... play ... " << endl;	
	while(true)
	{
		cin >> waitForUserInput;
		break;		
	}

	/* Stop an audio I/O stream. */
	err = Pa_StopStream(stream);
	if (err != paNoError) {
		cout << "--Error : Stop stream failed. Errorcode: " << err << " : " << Pa_GetErrorText(err) << endl;
		goto error;
	}

	/* Close an audio I/O stream. */
	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		cout << "--Error : Close stream failed. Errorcode: " << err << " : " << Pa_GetErrorText(err) << endl;
		goto error;
	}

	/* Terminate PortAudio */
	err = Pa_Terminate();
	if (err != paNoError) {
		cout << "--Error : Terminate stream failed. Errorcode: " << err << " : " << Pa_GetErrorText(err) << endl;
		goto error;
	}
	cout << "Stream terminated" << endl;
	

error:
	Pa_Terminate();
	fprintf(stderr, "An error occured while using the portaudio stream\n");
	fprintf(stderr, "Error number: %d\n", err);
	fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));

	// Halt the program to read messages written to screen if an error occurred
	/*while (true)
	{
		cin >> waitForUserInput;
		break;
	}*/


	return err;
	
}