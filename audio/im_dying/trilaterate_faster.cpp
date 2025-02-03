#include <iostream>
#include <vector>
#include <cmath>
#include <portaudio.h>
#include <Eigen/Dense>
#include <matplotlibcpp.h>

namespace plt = matplotlibcpp;
using namespace std;
using namespace Eigen;

// **🔹 Constants**
const int SAMPLE_RATE = 48000;  // Use a supported sample rate
const int DURATION = 2;  // Recording duration in seconds
const int NUM_MICS = 3;
const int FRAMES_PER_BUFFER = 1024;
const double SPEED_OF_SOUND = 343.0;  // Speed of sound in meters per second

// **🔹 Microphone positions**
map<int, vector<double>> MIC_POSITIONS = {
    {4, {0, 0}},   // Mic 4 (Reference)
    {2, {1, 0}},   // Mic 2
    {3, {0, 1}}    // Mic 3
};

// **🔹 Store recorded data**
vector<vector<float>> recordings(NUM_MICS);
vector<int> MIC_ORDER = {4, 2, 3};

// **🔹 PortAudio callback function**
static int recordCallback(const void *inputBuffer, void *outputBuffer,
                          unsigned long framesPerBuffer, const PaStreamCallbackTimeInfo *timeInfo,
                          PaStreamCallbackFlags statusFlags, void *userData) {
    auto *recordedData = (vector<vector<float>> *)userData;
    const float *in = (const float *)inputBuffer;

    if (inputBuffer != nullptr) {
        for (int i = 0; i < NUM_MICS; i++) {
            for (unsigned long j = 0; j < framesPerBuffer; j++) {
                recordedData->at(i).push_back(*in++);
            }
        }
    }
    return paContinue;
}

// **🔹 Record Audio**
void recordAudio() {
    Pa_Initialize();
    PaStream *stream;
    PaStreamParameters inputParameters;
    inputParameters.device = Pa_GetDefaultInputDevice();
    inputParameters.channelCount = NUM_MICS;
    inputParameters.sampleFormat = paFloat32;
    inputParameters.suggestedLatency = Pa_GetDeviceInfo(inputParameters.device)->defaultLowInputLatency;
    inputParameters.hostApiSpecificStreamInfo = nullptr;

    // **Open and start recording**
    Pa_OpenStream(&stream, &inputParameters, nullptr, SAMPLE_RATE, FRAMES_PER_BUFFER, paClipOff, recordCallback, &recordings);
    Pa_StartStream(stream);
    cout << "🎙️ Recording from all microphones for " << DURATION << " seconds..." << endl;
    Pa_Sleep(DURATION * 1000);
    Pa_StopStream(stream);
    Pa_CloseStream(stream);
    Pa_Terminate();
}

// **🔹 Detect first signal arrival**
int determineReferenceMic() {
    cout << "\n🎯 Determining Reference Mic...\n";
    vector<int> first_signal_samples(NUM_MICS, 0);

    for (int i = 0; i < NUM_MICS; i++) {
        float threshold = *max_element(recordings[i].begin(), recordings[i].end()) * 0.1;
        for (int j = 0; j < recordings[i].size(); j++) {
            if (abs(recordings[i][j]) > threshold) {
                first_signal_samples[i] = j;
                break;
            }
        }
        cout << "   - Mic " << MIC_ORDER[i] << ": First detected signal at sample " << first_signal_samples[i] << endl;
    }

    int refMicIndex = distance(first_signal_samples.begin(), min_element(first_signal_samples.begin(), first_signal_samples.end()));
    cout << "🎯 **New Reference Mic Assigned: Mic " << MIC_ORDER[refMicIndex] << "**\n";

    return MIC_ORDER[refMicIndex];
}

// **🔹 Compute cross-correlation for time lags**
vector<double> computeTimeLags(int refMic) {
    vector<double> time_lags(NUM_MICS - 1);
    vector<float> refSignal = recordings[refMic];

    cout << "\n🔍 Performing cross-correlation to find time lags...\n";
    for (int i = 1; i < NUM_MICS; i++) {
        vector<float> micSignal = recordings[i];

        double max_corr = 0;
        int max_lag = 0;
        for (int lag = -SAMPLE_RATE / 10; lag < SAMPLE_RATE / 10; lag++) {
            double corr = 0;
            for (int j = max(0, -lag); j < min((int)refSignal.size() - lag, (int)micSignal.size()); j++) {
                corr += refSignal[j] * micSignal[j + lag];
            }
            if (corr > max_corr) {
                max_corr = corr;
                max_lag = lag;
            }
        }
        double timeLag = max_lag / (double)SAMPLE_RATE;
        time_lags[i - 1] = timeLag;
        cout << "🔹 Time lag between Mic " << refMic << " and Mic " << MIC_ORDER[i] << ": " << timeLag << " seconds\n";
    }
    return time_lags;
}

// **🔹 Perform Trilateration**
vector<double> trilaterate(int refMic, vector<double> time_lags) {
    cout << "\n📍 Performing trilateration to locate sound source...\n";

    double r1 = abs(time_lags[0]) * SPEED_OF_SOUND;
    double r2 = abs(time_lags[1]) * SPEED_OF_SOUND;
    double r3 = 0;  // Reference mic has no lag

    cout << "   - Distances (meters): r1=" << r1 << ", r2=" << r2 << endl;

    Vector2d tower1(MIC_POSITIONS[refMic][0], MIC_POSITIONS[refMic][1]);
    Vector2d tower2(MIC_POSITIONS[MIC_ORDER[1]][0], MIC_POSITIONS[MIC_ORDER[1]][1]);
    Vector2d tower3(MIC_POSITIONS[MIC_ORDER[2]][0], MIC_POSITIONS[MIC_ORDER[2]][1]);

    Vector2d A = 2 * (tower2 - tower1);
    Vector2d B = 2 * (tower3 - tower1);
    double C = pow(r1, 2) - pow(r2, 2) - tower1.dot(tower1) + tower2.dot(tower2);
    double D = pow(r1, 2) - pow(r3, 2) - tower1.dot(tower1) + tower3.dot(tower3);

    Matrix2d A_matrix;
    A_matrix << A[0], B[0], A[1], B[1];
    Vector2d b_vector(C, D);

    Vector2d estimated_position = A_matrix.colPivHouseholderQr().solve(b_vector);
    cout << "✅ Estimated position of the sound source: [" << estimated_position[0] << ", " << estimated_position[1] << "]\n";

    return {estimated_position[0], estimated_position[1]};
}

// **🔹 Plot localization**
void plotLocalization(vector<double> estimated_position) {
    plt::figure_size(800, 800);
    plt::scatter({0, 1, 0}, {0, 0, 1}, 100, {{"color", "red"}});
    plt::scatter({estimated_position[0]}, {estimated_position[1]}, 150, {{"color", "magenta"}});
    plt::title("Sound Source Localization");
    plt::xlabel("X Position (meters)");
    plt::ylabel("Y Position (meters)");
    plt::grid(true);
    plt::save("sound_source_localization_debug.png");
    cout << "✅ Saved: sound_source_localization_debug.png\n";
}

int main() {
    recordAudio();
    int refMic = determineReferenceMic();
    vector<double> time_lags = computeTimeLags(refMic);
    vector<double> estimated_position = trilaterate(refMic, time_lags);
    plotLocalization(estimated_position);
}

