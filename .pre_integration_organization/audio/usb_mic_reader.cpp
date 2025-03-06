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
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        std::cout << in[i] << " ";
    }
    std::cout << std::endl;
    return paContinue;
}

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    PaStream* stream;
    err = Pa_OpenDefaultStream(&stream, 1, 0, paFloat32, SAMPLE_RATE,
                               FRAMES_PER_BUFFER, audioCallback, nullptr);
    if (err != paNoError) {
        std::cerr << "Failed to open stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to start stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    std::cout << "Recording from USB microphone. Press Enter to stop..." << std::endl;
    std::cin.get();

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Failed to stop stream: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_CloseStream(stream);
    Pa_Terminate();
    return 0;
}

