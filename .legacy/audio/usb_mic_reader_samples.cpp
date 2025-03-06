#include <iostream>
#include <portaudio.h>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData) {
    const float* in = (const float*)inputBuffer;

    if (inputBuffer == nullptr) {
        return paContinue;
    }

    // Print raw audio samples
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        //std::cout << in[i] << " ";
    }
    std::cout << std::endl;

    return paContinue;
}

int main() {
    PaError err;

    // Initialize PortAudio
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    // Print default input device info
    const PaDeviceInfo* inputDeviceInfo = Pa_GetDeviceInfo(Pa_GetDefaultInputDevice());
    if (inputDeviceInfo == nullptr) {
        std::cerr << "Failed to get input device info" << std::endl;
        Pa_Terminate();
        return 1;
    }

    std::cout << "Using input device: " << inputDeviceInfo->name << std::endl;
    std::cout << "Max input channels: " << inputDeviceInfo->maxInputChannels << std::endl;
    std::cout << "Default sample rate: " << inputDeviceInfo->defaultSampleRate << " Hz" << std::endl;

    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = 1; // Mono input
    inputParameters.sampleFormat = paFloat32; // 32-bit floating-point
    inputParameters.suggestedLatency = inputDeviceInfo->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    std::cout << "Sample format: 32-bit floating-point (paFloat32)" << std::endl;
    std::cout << "Suggested latency: " << inputParameters.suggestedLatency << " seconds" << std::endl;

    // Open stream
    PaStream* stream;
    err = Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "Failed to open stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    // Start stream
    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    std::cout << "Recording... Press Enter to stop." << std::endl;
    std::cin.get();

    // Stop stream
    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to stop stream: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
}

