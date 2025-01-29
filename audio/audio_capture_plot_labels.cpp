// audio_capture_plot_labeled.cpp

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
#define NUM_SECONDS 10            // Changed from 1 to 10 seconds
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
    const int MARGIN_LEFT = 70;    // Increased to accommodate Y-axis label
    const int MARGIN_RIGHT = 50;
    const int MARGIN_TOP = 50;
    const int MARGIN_BOTTOM = 70;  // Increased to accommodate X-axis label

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

    // Load a font for labels
    XFontStruct* font = XLoadQueryFont(display, "fixed");
    if (!font) {
        std::cerr << "Unable to load font. Labels may not appear correctly." << std::endl;
    } else {
        XSetFont(display, gc, font->fid);
    }

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

    // Add axis labels
    if (font) {
        // Y-axis label: "Amplitude"
        std::string yLabel = "Amplitude";
        // Calculate position for Y-axis label (rotated)
        // X11 does not support rotated text easily, so we'll place it vertically
        // by drawing each character separately
        int yLabelX = 10; // Left margin
        int yLabelY = MARGIN_TOP + (HEIGHT - MARGIN_TOP - MARGIN_BOTTOM) / 2;
        for (size_t i = 0; i < yLabel.size(); ++i) {
            std::string letter(1, yLabel[i]);
            XDrawString(display, win, gc, yLabelX, yLabelY + i * 12, letter.c_str(), 1);
        }

        // X-axis label: "Time (s)"
        std::string xLabel = "Time (s)";
        int xLabelWidth = XTextWidth(font, xLabel.c_str(), xLabel.size());
        int xLabelX = MARGIN_LEFT + (WIDTH - MARGIN_LEFT - MARGIN_RIGHT) / 2 - xLabelWidth / 2;
        int xLabelY = HEIGHT - 20; // Below the X-axis
        XDrawString(display, win, gc, xLabelX, xLabelY, xLabel.c_str(), xLabel.size());
    }

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

    // Calculate total time in seconds
    double totalTime = static_cast<double>(dataSize) / SAMPLE_RATE;

    // Plot each sample
    for (size_t i = 1; i < dataSize; ++i) {
        double currentTime = static_cast<double>(i) / SAMPLE_RATE;
        double prevTime = static_cast<double>(i - 1) / SAMPLE_RATE;

        int x1 = MARGIN_LEFT + static_cast<int>((prevTime / totalTime) * plotWidth);
        int y1 = MARGIN_TOP + static_cast<int>((1.0 - (data[i - 1] / maxSample)) * plotHeight / 2);
        int x2 = MARGIN_LEFT + static_cast<int>((currentTime / totalTime) * plotWidth);
        int y2 = MARGIN_TOP + static_cast<int>((1.0 - (data[i] / maxSample)) * plotHeight / 2);
        XDrawLine(display, win, gc, x1, y1, x2, y2);
    }

    // Optional: Add tick marks and numerical labels (basic implementation)
    if (font) {
        // X-axis ticks and labels (every 2 seconds)
        int numXTicks = 5;
        for (int i = 0; i <= numXTicks; ++i) {
            double tickTime = (totalTime / numXTicks) * i;
            int x = MARGIN_LEFT + static_cast<int>((tickTime / totalTime) * plotWidth);
            int yStart = HEIGHT - MARGIN_BOTTOM;
            int yEnd = HEIGHT - MARGIN_BOTTOM + 5;
            XDrawLine(display, win, gc, x, yStart, x, yEnd);

            // Draw tick label
            std::string tickLabel = std::to_string(static_cast<int>(tickTime));
            int labelWidth = XTextWidth(font, tickLabel.c_str(), tickLabel.size());
            int labelX = x - labelWidth / 2;
            int labelY = yEnd + 15;
            XDrawString(display, win, gc, labelX, labelY, tickLabel.c_str(), tickLabel.size());
        }

        // Y-axis ticks and labels (every 0.5 amplitude units)
        int numYTicks = 4;
        for (int i = 0; i <= numYTicks; ++i) {
            double tickAmplitude = (static_cast<double>(i) / numYTicks) * maxSample;
            int y = MARGIN_TOP + static_cast<int>((1.0 - (tickAmplitude / maxSample)) * plotHeight / 2);
            int xStart = MARGIN_LEFT;
            int xEnd = MARGIN_LEFT - 5;
            XDrawLine(display, win, gc, xEnd, y, xStart, y);

            // Draw tick label
            std::string tickLabel = std::to_string(tickAmplitude);
            int labelWidth = XTextWidth(font, tickLabel.c_str(), tickLabel.size());
            int labelX = xEnd - labelWidth - 5;
            int labelY = y + 5;
            XDrawString(display, win, gc, labelX, labelY, tickLabel.c_str(), tickLabel.size());
        }
    }

    // Flush the drawing
    XFlush(display);

    // Inform the user
    std::cout << "Plot displayed with axis labels. Close the window or press a key to exit." << std::endl;

    // Wait for a key press or window close
    bool done = false;
    while (!done) {
        XNextEvent(display, &event);
        if (event.type == KeyPress || event.type == DestroyNotify) {
            done = true;
        }
    }

    // Cleanup
    if (font) {
        XFreeFont(display, font);
    }
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

    std::cout << "Recording started for " << NUM_SECONDS << " seconds. Please speak into the microphone..." << std::endl;

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
    plotAudioData(data.samples, "audio_plot_labeled.png");

    return 0;
}

