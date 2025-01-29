// audio_capture_plot.cpp

#include <iostream>
#include <vector>
#include <string>
#include <portaudio.h>
#include <sndfile.h>
#include <X11/Xlib.h>
#include <unistd.h>
#include <cmath>
#include <cstring>
#include <chrono>
#include <thread>

// Constants for audio
#define SAMPLE_RATE 44100
#define FRAMES_PER_BUFFER 256
#define NUM_SECONDS 1
#define NUM_CHANNELS 1
#define SAMPLE_TYPE paFloat32
typedef float SAMPLE;

// Function to list USB input devices and select the first one
int getUSBInputDevice() {
    int numDevices = Pa_GetDeviceCount();
    if (numDevices < 0) {
        std::cerr << "PortAudio error: " << Pa_GetErrorText(numDevices) << std::endl;
        return -1;
    }

    for (int i = 0; i < numDevices; i++) {
        const PaDeviceInfo* deviceInfo = Pa_GetDeviceInfo(i);
        if (deviceInfo->maxInputChannels > 0 && std::string(deviceInfo->name).find("USB") != std::string::npos) {
            std::cout << "Selected USB Device: " << deviceInfo->name << std::endl;
            return i;
        }
    }

    std::cerr << "No USB input device found." << std::endl;
    return -1;
}

// Structure to hold audio data
struct AudioData {
    std::vector<SAMPLE> samples;
    bool recording;
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
        // No input
        return paContinue;
    }

    // Append samples to the buffer
    data->samples.insert(data->samples.end(), in, in + framesPerBuffer);

    return paContinue;
}

// Function to write WAV file using libsndfile
bool writeWavFile(const std::string& filename, const std::vector<SAMPLE>& data, int sampleRate, int channels) {
    SF_INFO sfinfo;
    std::memset(&sfinfo, 0, sizeof(sfinfo));
    sfinfo.samplerate = sampleRate;
    sfinfo.channels = channels;
    sfinfo.format = SF_FORMAT_WAV | SF_FORMAT_PCM_16;

    SNDFILE* outfile = sf_open(filename.c_str(), SFM_WRITE, &sfinfo);
    if (!outfile) {
        std::cerr << "Error opening output file: " << sf_strerror(NULL) << std::endl;
        return false;
    }

    // Convert float samples to short for PCM 16
    std::vector<short> intData(data.size());
    for (size_t i = 0; i < data.size(); ++i) {
        float sample = data[i];
        if (sample > 1.0f) sample = 1.0f;
        if (sample < -1.0f) sample = -1.0f;
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

// Function to plot audio data using X11
void plotAudioData(const std::vector<SAMPLE>& data, const std::string& filename) {
    // Window dimensions
    const int WIDTH = 800;
    const int HEIGHT = 600;
    const int MARGIN_LEFT = 50;
    const int MARGIN_RIGHT = 50;
    const int MARGIN_TOP = 50;
    const int MARGIN_BOTTOM = 50;

    // Initialize X11
    Display* display = XOpenDisplay(NULL);
    if (display == NULL) {
        std::cerr << "Unable to open X display for plotting." << std::endl;
        return;
    }

    int screen = DefaultScreen(display);
    Window win = XCreateSimpleWindow(display, RootWindow(display, screen),
                                     10, 10, WIDTH, HEIGHT, 1,
                                     BlackPixel(display, screen),
                                     WhitePixel(display, screen));

    // Select input events
    XSelectInput(display, win, ExposureMask | KeyPressMask);
    XMapWindow(display, win);

    // Create Graphics Context
    GC gc = XCreateGC(display, win, 0, NULL);
    XSetForeground(display, gc, BlackPixel(display, screen));

    // Wait for the window to be exposed
    XEvent event;
    do {
        XNextEvent(display, &event);
    } while (event.type != Expose);

    // Draw axes
    // X-axis
    XDrawLine(display, win, gc, MARGIN_LEFT, HEIGHT - MARGIN_BOTTOM,
              WIDTH - MARGIN_RIGHT, HEIGHT - MARGIN_BOTTOM);
    // Y-axis
    XDrawLine(display, win, gc, MARGIN_LEFT, HEIGHT - MARGIN_BOTTOM,
              MARGIN_LEFT, MARGIN_TOP);

    // Plot the audio waveform
    size_t dataSize = data.size();
    if (dataSize < 2) {
        std::cerr << "Not enough data to plot." << std::endl;
        XFreeGC(display, gc);
        XDestroyWindow(display, win);
        XCloseDisplay(display);
        return;
    }

    // Find the maximum absolute sample value for scaling
    float maxSample = 0.0f;
    for (const auto& sample : data) {
        if (std::abs(sample) > maxSample) {
            maxSample = std::abs(sample);
        }
    }
    if (maxSample == 0.0f) maxSample = 1.0f; // Prevent division by zero

    // Define plot area
    int plotWidth = WIDTH - MARGIN_LEFT - MARGIN_RIGHT;
    int plotHeight = HEIGHT - MARGIN_TOP - MARGIN_BOTTOM;

    // Plot each sample
    for (size_t i = 1; i < dataSize; ++i) {
        int x1 = MARGIN_LEFT + static_cast<int>((i - 1) * plotWidth / static_cast<double>(dataSize - 1));
        int y1 = MARGIN_TOP + static_cast<int>((1.0 - (data[i - 1] / maxSample)) * plotHeight / 2);
        int x2 = MARGIN_LEFT + static_cast<int>(i * plotWidth / static_cast<double>(dataSize - 1));
        int y2 = MARGIN_TOP + static_cast<int>((1.0 - (data[i] / maxSample)) * plotHeight / 2);
        XDrawLine(display, win, gc, x1, y1, x2, y2);
    }

    // Flush the drawing
    XFlush(display);

    // Optional: Save the plot as an image using XImage (requires additional implementation)
    // For simplicity, this example only displays the plot in a window.

    std::cout << "Plot displayed. Close the window or press a key to exit." << std::endl;

    // Wait for a key press or window close
    bool done = false;
    while (!done) {
        XNextEvent(display, &event);
        if (event.type == KeyPress || event.type == DestroyNotify) {
            done = true;
        }
    }

    // Cleanup
    XFreeGC(display, gc);
    XDestroyWindow(display, win);
    XCloseDisplay(display);
}

int main() {
    PaError err;
    err = Pa_Initialize();
    if (err != paNoError) {
        std::cerr << "PortAudio initialization error: " << Pa_GetErrorText(err) << std::endl;
        return 1;
    }

    int deviceIndex = getUSBInputDevice();
    if (deviceIndex == -1) {
        Pa_Terminate();
        return 1;
    }

    PaStreamParameters inputParameters;
    inputParameters.device = deviceIndex;
    inputParameters.channelCount = NUM_CHANNELS;
    inputParameters.sampleFormat = SAMPLE_TYPE;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(deviceIndex)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = NULL;

    AudioData data;
    data.recording = true;

    PaStream* stream;
    err = Pa_OpenStream(&stream,
                        &inputParameters,
                        NULL, // no output
                        SAMPLE_RATE,
                        FRAMES_PER_BUFFER,
                        paClipOff,
                        audioCallback,
                        &data);
    if (err != paNoError) {
        std::cerr << "Error opening stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_Terminate();
        return 1;
    }

    err = Pa_StartStream(stream);
    if (err != paNoError) {
        std::cerr << "Error starting stream: " << Pa_GetErrorText(err) << std::endl;
        Pa_CloseStream(stream);
        Pa_Terminate();
        return 1;
    }

    std::cout << "Recording started. Please speak into the microphone..." << std::endl;

    // Record for NUM_SECONDS seconds
    std::this_thread::sleep_for(std::chrono::seconds(NUM_SECONDS));

    err = Pa_StopStream(stream);
    if (err != paNoError) {
        std::cerr << "Error stopping stream: " << Pa_GetErrorText(err) << std::endl;
    }

    Pa_CloseStream(stream);
    Pa_Terminate();

    std::cout << "Recording finished. Number of samples recorded: " << data.samples.size() << std::endl;

    // Save to WAV file
    std::string wavFilename = "captured_audio.wav";
    if (writeWavFile(wavFilename, data.samples, SAMPLE_RATE, NUM_CHANNELS)) {
        std::cout << "Audio data saved to " << wavFilename << std::endl;
    } else {
        std::cerr << "Failed to save audio data to WAV file." << std::endl;
    }

    // Plot the audio data
    plotAudioData(data.samples, "audio_plot.png");

    return 0;
}

