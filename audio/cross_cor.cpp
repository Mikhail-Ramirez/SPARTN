#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <sndfile.h>  // Library for reading WAV files

using namespace std;
using namespace cv;

// Function to read audio file using libsndfile
vector<float> readAudio(const string &filename, int &sampleRate) {
    SNDFILE *file;
    SF_INFO sfinfo;
    file = sf_open(filename.c_str(), SFM_READ, &sfinfo);
    
    if (!file) {
        cerr << "Error opening audio file: " << filename << endl;
        exit(1);
    }

    sampleRate = sfinfo.samplerate;
    vector<float> audioData(sfinfo.frames);
    sf_read_float(file, audioData.data(), sfinfo.frames);
    sf_close(file);

    return audioData;
}

// Function to compute cross-correlation
vector<float> crossCorrelation(const vector<float> &x, const vector<float> &y) {
    int len = x.size() + y.size() - 1;
    vector<float> result(len, 0.0);

    for (int i = 0; i < x.size(); i++) {
        for (int j = 0; j < y.size(); j++) {
            result[i + j] += x[i] * y[j];
        }
    }

    return result;
}

int main() {
    int fs1, fs2;
    vector<float> x = readAudio("MATLAB_CrossCorrTest/LaptopRec_Stereo.wav", fs1);
    vector<float> y = readAudio("MATLAB_CrossCorrTest/PhoneRec_Stereo.wav", fs2);

    int Ix = x.size();
    int Iy = y.size();
    int minSamples = min(Ix, Iy);

    // Select the range of samples for plotting
    vector<float> x_trimmed(x.begin(), x.begin() + minSamples);
    vector<float> y_trimmed(y.begin(), y.begin() + minSamples);

    // Compute cross-correlation
    vector<float> C1 = crossCorrelation(x_trimmed, y_trimmed);

    // Generate lag values
    vector<float> lag1(C1.size());
    for (int i = 0; i < C1.size(); i++) {
        lag1[i] = (i - minSamples) / static_cast<float>(fs1);
    }

    // Plot using OpenCV
    int width = 800, height = 300;
    
    // Create images for plots
    Mat plot1(height, width, CV_8UC3, Scalar(255, 255, 255));
    Mat plot2(height, width, CV_8UC3, Scalar(255, 255, 255));
    Mat plot3(height, width, CV_8UC3, Scalar(255, 255, 255));

    // Plot signals and correlation
    polylines(plot1, {x_trimmed}, false, Scalar(0, 0, 255), 1);
    polylines(plot2, {y_trimmed}, false, Scalar(0, 255, 0), 1);
    polylines(plot3, {C1}, false, Scalar(255, 0, 0), 1);

    // Show plots
    imshow("Audio Signal 1", plot1);
    imshow("Audio Signal 2", plot2);
    imshow("Cross-Correlation", plot3);

    waitKey(0);
    return 0;
}

