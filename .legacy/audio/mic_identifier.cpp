#include <iostream>
#include <portaudio.h>
#include <sndfile.h>
#include <vector>
#include <string>
#include <cstring>  // For memset()
#include <X11/Xlib.h>
#include <unistd.h>
#include <cmath>
#include <chrono>
#include <thread>

#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define NUM_SECONDS 10
#define NUM_CHANNELS 1
#define SAMPLE_TYPE paFloat32
typedef float SAMPLE;

// Microphone mapping based on USB device IDs
const std::vector<std::pair<int, std::string>> micMapping = {
    {39, "mic1.wav"},
    {40, "mic2.wav"},
    {41, "mic3.wav"}
};

// Function to get PortAudio device index for a given USB device ID
int getMicDeviceIndex(int usbDeviceID) {
    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(numDevices) << std::endl;
        return -1;
    }

    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxInputChannels > 0 && std::string(deviceInfo->name).find("USB") != std::string::npos) {
            // Convert device name to std::string before calling .find()
            std::string devName = std::string(deviceInfo->name);
            if (devName.find(std::to_string(usbDeviceID)) != std::string::npos) {
                return i;
            }
        }
    }

    std::cerr << "No matching device found for USB Device ID " << usbDeviceID << std::endl;
    return -1;
}

// Structure to hold audio data
struct AudioData {
    std::vector<SAMPLE> samples;
};

// PortAudio callback function
static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags,
                         void* userData) {
    AudioData* data = (AudioData*)userData;
    const SAMPLE* in = (const SAMPLE*)inputBuffer;

    if (inputBuffer == nullptr) {
        return paContinue;
    }

    data->samples.insert(data->samples.end(), in, in + framesPerBuffer);
    return paContinue;
}

// Function to write WAV file
bool writeWavFile(const std::string& filename, const std::vector<SAMPLE>& data) {
    SF_INFO sfinfo;
    memset(&sfinfo, 0, sizeof(sfinfo));  // Fixed: Removed std:: namespace
    sfinfo.samplerate = SAMPLE_RATE;
    sfinfo.channels = NUM_CHANNELS;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(filename.c_str(), SFM_WRITE, &sfinfo);
    if (!outfile) {
        std::cerr << "Error opening output file: " << sf_strerror(NULL) << std::endl;
        return false;
    }

    std::vector<short> intData(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        float sample = data[i];
        intData[i] = static_cast<short>(sample * 32767);
    }

    sf_count_t count = sf_write_short(outfile, intData.data(), intData.size());
    if (count != static_cast<sf_count_t>(intData.size())) {
        std::cerr << "Error writing to WAV file." << std::endl;
        sf_close(outfile);
        return false;
    }

    sf_close(outfile);
    return true;
}

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    for (const auto& mic : micMapping) {
        int deviceIndex = getMicDeviceIndex(mic.first);
        if (deviceIndex == -1) continue;

        PaStreamParameters inputParameters;
        inputParameters.device = deviceIndex;
        inputParameters.channelCount = NUM_CHANNELS;
        inputParameters.sampleFormat = SAMPLE_TYPE;
        inputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultLowInputLatency;
        inputParameters.hostApiSpecificStreamInfo = NULL;

        AudioData data;
        PaStream* stream;
        err = Pa_OpenStream(&stream, &inputParameters, NULL, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, &data);
        if (err != paNoError) {
            std::cerr << "Error opening stream: " << Pa_GetErrorText(err) << std::endl;
            continue;
        }

        err = Pa_StartStream(stream);
        if (err != paNoError) {
            std::cerr << "Error starting stream: " << Pa_GetErrorText(err) << std::endl;
            Pa_CloseStream(stream);
            continue;
        }

        std::cout << "Recording from Mic (USB Device " << mic.first << ") for " << NUM_SECONDS << " seconds..." << std::endl;
        std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

        Pa_StopStream(stream);
        Pa_CloseStream(stream);

        std::cout << "Saving to " << mic.second << std::endl;
        writeWavFile(mic.second, data.samples);
    }

    Pa_Terminate();
    return 0;
}

