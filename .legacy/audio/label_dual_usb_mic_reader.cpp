#include <iostream>
#include <portaudio.h>
#include <vector>
#include <string>
#include <algorithm>
#include <thread>
#include <chrono>
#include <fstream>

std::ofstream fileCamera1("camera1_audio_data.txt");
std::ofstream fileCamera2("camera2_audio_data.txt");


#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256

struct DeviceInfo {
    int deviceIndex;
    std::string name;
    int maxInputChannels;
    double defaultSampleRate;
    double suggestedLatency;
};

struct CallbackData {
    std::string label; // Camera label
};

static int audioCallback(const void* inputBuffer, void* outputBuffer,
                         unsigned long framesPerBuffer,
                         const PaStreamCallbackTimeInfo* timeInfo,
                         PaStreamCallbackFlags statusFlags, void* userData) {
    const float* in = (const float*)inputBuffer;
    CallbackData* callbackData = (CallbackData*)userData;

    if (inputBuffer == nullptr) {
        std::cerr << callbackData->label << ": No input data available." << std::endl;
        return paContinue;
    }

    // Select the correct file based on the label
    std::ofstream* outFile = callbackData->label == "Camera 1" ? &fileCamera1 : &fileCamera2;

    // Write audio data to the file
    *outFile << "=== " << callbackData->label << " ===" << std::endl;
    *outFile << "Audio Data [" << std::endl;

    unsigned long samplesPerLine = 10; // Limit samples per line for readability
    for (unsigned long i = 0; i < framesPerBuffer; i++) {
        *outFile << in[i];
        if (i < framesPerBuffer - 1) {
            *outFile << ", ";
        }
        if ((i + 1) % samplesPerLine == 0 && i < framesPerBuffer - 1) {
            *outFile << std::endl;
        }
    }
    *outFile << "\n]" << std::endl;

    return paContinue;
}


void listUSBInputDevices(std::vector<DeviceInfo>& deviceList) {
    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        std::cerr << "Error: " << Pa_GetErrorText(numDevices) << std::endl;
        return;
    }

    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxInputChannels > 0 && std::string(deviceInfo->name).find("USB") != std::string::npos) {
            DeviceInfo info = {
                i,
                deviceInfo->name,
                deviceInfo->maxInputChannels,
                deviceInfo->defaultSampleRate,
                deviceInfo->defaultLowInputLatency
            };
            deviceList.push_back(info);
        }
    }
}

void printDeviceInfo(const DeviceInfo& device) {
    std::cout << "Device Index: " << device.deviceIndex << std::endl;
    std::cout << "Device Name: " << device.name << std::endl;
    std::cout << "Max Input Channels: " << device.maxInputChannels << std::endl;
    std::cout << "Default Sample Rate: " << device.defaultSampleRate << " Hz" << std::endl;
    std::cout << "Suggested Latency: " << device.suggestedLatency << " seconds" << std::endl;
    std::cout << "---------------------------" << std::endl;
}

int main() {
    PaError err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    std::vector<DeviceInfo> deviceList;
    listUSBInputDevices(deviceList);

    if (deviceList.size() < 2) {
        std::cerr << "Error: Expected 2 USB devices but found " << deviceList.size() << "." << std::endl;
        Pa_Terminate();
        return 1;
    }

    std::cout << "Filtered USB Devices:" << std::endl;
    for (const auto& device : deviceList) {
        printDeviceInfo(device);
    }

    PaStreamParameters inputParameters1, inputParameters2;
    const auto& device1 = deviceList[0];
    const auto& device2 = deviceList[1];

    CallbackData callbackData1{"Camera 1"};
    CallbackData callbackData2{"Camera 2"};

    inputParameters1.device = device1.deviceIndex;
    inputParameters1.channelCount = 1; // Mono input
    inputParameters1.sampleFormat = paFloat32; // 32-bit floating-point
    inputParameters1.suggestedLatency = device1.suggestedLatency;
    inputParameters1.hostApiSpecificStreamInfo = nullptr;

    inputParameters2.device = device2.deviceIndex;
    inputParameters2.channelCount = 1; // Mono input
    inputParameters2.sampleFormat = paFloat32; // 32-bit floating-point
    inputParameters2.suggestedLatency = device2.suggestedLatency;
    inputParameters2.hostApiSpecificStreamInfo = nullptr;

    PaStream *stream1, *stream2;
    err = Pa_OpenStream(&stream1, &inputParameters1, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, &callbackData1);
    if (err != paNoError) {
        std::cerr << "Failed to open stream for device 1: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    err = Pa_OpenStream(&stream2, &inputParameters2, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, audioCallback, &callbackData2);
    if (err != paNoError) {
        std::cerr << "Failed to open stream for device 2: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream1);
        Pa_Terminate();
        return 1;
    }

    Pa_StartStream(stream1);
    Pa_StartStream(stream2);

    std::cout << "Recording for 1 second..." << std::endl;
    std::this_thread::sleep_for(std::chrono::seconds(1));

    Pa_StopStream(stream1);
    Pa_StopStream(stream2);
    Pa_CloseStream(stream1);
    Pa_CloseStream(stream2);
    Pa_Terminate();

    std::cout << "Recording finished." << std::endl;
    return 0;
}

