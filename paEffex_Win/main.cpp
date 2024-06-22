#include "portaudio.h"
#include "pa_asio.h"
#include "EffectManager.h"
#include <iostream>
#include <string>

#include "DelayEffect.h"
#include "ChorusEffect.h"
//#include "MsgStruct.h"

using namespace std;

// Using a global object to hold the effects for simplicity.
EffectManager effectManager;

//MsgFromUser UDPmsg;

#define SAMPLE_RATE (44100)
#define FRAMES_PER_BUFFER (64)

int processAudioStreamCallback(const void* input, void* output, unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo*, PaStreamCallbackFlags, void* userData) {

	effectManager.applyEffect(input, output, framesPerBuffer);
	return 0;
}

void ExitProgram(PaError err, string errorMsg) {
	
	
	cout << "Exiting program" << endl;
	
	if(!errorMsg.empty())
		cout << "Error: " << errorMsg << endl;

	if (err != paNoError) {

		fprintf(stderr, "Error number: %d\n", err);
		fprintf(stderr, "Error message: %s\n", Pa_GetErrorText(err));
	}

	Pa_Terminate();
	exit(0);
}

int main(void);
int main(void) {

	/* Stream parameters */
	PaStream* stream;
	PaError err = paNoError;
	PaStreamParameters inputParameters, outputParameters;
	PaStreamFlags streamFlags = paNoFlag; /*paClipOff paDitherOff paNeverDropInput paPrimeOutputBuffersUsingStreamCallback paPlatformSpecificFlags*/
	PaHostApiInfo APIInfo;


	/* PA info */
	const PaDeviceInfo* inputInfo;
	const PaDeviceInfo* outputInfo;
	PaHostApiIndex ApiIndex;
	const PaHostApiInfo* HostApiInfo;

	/* Internal variables for main()*/
	string waitForUserInput;


	/* Initialize PortAudio */
	err = Pa_Initialize();
	if (err != paNoError) {
		ExitProgram(err, "PortAudio failed to initialize");
	}

	/* Initialize Stream Parameters */
	auto deviceCount = Pa_GetDeviceCount();
	inputParameters.device = 1; // Pa_GetDefaultInputDevice();	
	outputParameters.device = 1; //	Pa_GetDefaultOutputDevice();

	if (inputParameters.device == paNoDevice) {
		ExitProgram(err, "No default input device");
	}
	if (outputParameters.device == paNoDevice) {
		ExitProgram(err, "No default output device");
	}
	inputInfo = Pa_GetDeviceInfo(inputParameters.device);
	outputInfo = Pa_GetDeviceInfo(outputParameters.device);

	inputParameters.channelCount = inputInfo->maxInputChannels;
	outputParameters.channelCount = outputInfo->maxOutputChannels;
	inputParameters.sampleFormat = paFloat32; /*+paNonInterleaved;*/
	outputParameters.sampleFormat = paFloat32; /*+paNonInterleaved;*/
	inputParameters.suggestedLatency = inputInfo->defaultLowInputLatency;
	outputParameters.suggestedLatency = outputInfo->defaultLowOutputLatency;
	inputParameters.hostApiSpecificStreamInfo = NULL;
	outputParameters.hostApiSpecificStreamInfo = NULL;

	/*-----------------------------------------------------------------------------*/
	/*DEBUG INFO*/
	// Show I/O paramteres	
	ApiIndex = Pa_GetDefaultHostApi();
	HostApiInfo = Pa_GetHostApiInfo(ApiIndex);
	/*cout << "INPUT parameters---------" << endl;
	cout << "     input DevID:        " << inputParameters.device << endl;
	cout << "     Name:               " << inputInfo->name << endl;
	cout << "     LL:                 " << inputInfo->defaultLowInputLatency << endl;
	cout << "     HL:                 " << inputInfo->defaultHighInputLatency << endl;
	cout << "     max in ch:          " << inputInfo->maxInputChannels << endl;
	cout << "OUTPUT parameters--------" << endl;
	cout << "     output DevID:       " << outputParameters.device << endl;
	cout << "     Name:               " << outputInfo->name << endl;
	cout << "     LL:                 " << outputInfo->defaultLowOutputLatency << endl;
	cout << "     HL:                 " << outputInfo->defaultHighOutputLatency << endl;
	cout << "     max out ch:         " << outputInfo->maxOutputChannels << endl;
	cout << "     default samplerate: " << outputInfo->defaultSampleRate << endl;
	cout << "-------------------------" << endl;
	cout << "   Default API index:    " << ApiIndex << endl;
	cout << "   Default Host API info " << endl;
	cout << "      Input device:      " << HostApiInfo->defaultInputDevice << endl;
	cout << "      Output device:     " << HostApiInfo->defaultOutputDevice << endl;
	cout << "      Device count:      " << HostApiInfo->deviceCount << endl;
	cout << "      Name:              " << HostApiInfo->name << endl;
	cout << "      Struct ver:        " << HostApiInfo->structVersion << endl;
	cout << "      Type:              " << HostApiInfo->type << endl;*/



	// Example show ASIO panel
	//PaAsio_ShowControlPanel(inputParameters.device, nullptr);
	/*----------------------------------------------------------------------------------*/

	/* Create Effects */
	effectManager.setEffect(new DelayEffect());
	effectManager.setEffect(new ChorusEffect());

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
		ExitProgram(err, "Failed to open stream");
	}
	cout << " -- Opening stream successful -- " << endl;

	/* Start an audio I/O stream. */
	err = Pa_StartStream(stream);
	if (err != paNoError) {
		ExitProgram(err, "Failed to start stream");
	}
	cout << " -- Starting stream successful -- " << endl;

	/*  -- Block thread until user hits enter -- */
	cout << endl << "... play ... " << endl;
	while (true)
	{
		cin >> waitForUserInput;
		break;
	}

	/* Stop an audio I/O stream. */
	err = Pa_StopStream(stream);
	if (err != paNoError) {
		ExitProgram(err, "Failed to stop stream");
	}

	/* Close an audio I/O stream. */
	err = Pa_CloseStream(stream);
	if (err != paNoError) {
		ExitProgram(err, "Failed to close stream");
	}

	/* Terminate PortAudio */
	err = Pa_Terminate();
	if (err != paNoError) {
		ExitProgram(err, "Failed to terminate stream");
	}

	// Halt the program to read messages written to screen if an error occurred
	//while (true)
	//{
	//	cin >> waitForUserInput;
	//	break;
	//}

	return 0;
}