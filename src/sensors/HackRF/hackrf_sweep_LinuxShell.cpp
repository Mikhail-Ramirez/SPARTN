#include <iostream>
#include <string>
#include <stdlib.h>

#include <vector>

#include <bits/stdc++.h>

#include <algorithm>

#include <cstdio>
#include <memory>
#include <stdexcept>

using namespace std;

std::string exec(const char* cmd);
vector<vector<string>> breakUp(string);
vector<tuple<uint64_t, int>> process(vector<vector<string>>);
void printout(vector<tuple<uint64_t, int>>);
string GetStdoutFromCommand(string cmd);

const int TOLERANCE = 100;

int main() {
	cout << "Starting...  Press Ctrl-C to exit!" << endl;

    //Create the command strings to be executed.

    string nineMHzCommandRaw = "hackrf_sweep -f 902:928 -N 1 -l 40 -g 8 2>/dev/null";
    string twoGHzCommandRaw = "hackrf_sweep -f 2400:2495 -N 1 -l 40 -g 8 2>/dev/null";
    string fiveGHzCommandRaw = "hackrf_sweep -f 5150:5895 -N 1 -l 40 -g 8 2>/dev/null";

    const char* nineMHzCommand = nineMHzCommandRaw.c_str();
    const char* twoGHzCommand = twoGHzCommandRaw.c_str();
    const char* fiveGHzCommand = fiveGHzCommandRaw.c_str();


    //Create the local variables to store the output of the command being run.

    string nineMHz;
    string twoGHz;
    string fiveGHz;

    vector<vector<string>> nineMHz_sub;
    vector<vector<string>> twoGHz_sub;
    vector<vector<string>> fiveGHz_sub;

    vector<tuple<uint64_t, int>> nineMHz_processed;
    vector<tuple<uint64_t, int>> twoGHz_processed;
    vector<tuple<uint64_t, int>> fiveGHz_processed;

    ///////////////////Main loop
    //1.  Run the command.
    //2.  Break the output between commas to get a parsable output.
    //3.  Process the parsable output into integers of vectors and decibels.
    //4.  Due to how hackrf_sweep is multithreaded, the output is jumbled.  Sort it!
    //5.  Print it out to show it works.

    //TODO: Put most of the main loop into a header file so it can be called from other code easily.
    //TODO: Rework how the process function processes data, preferibly so that it can parse it regardless of how many entries there are in a line.
    //          Each line has a date, a time, a starting freq, and end freq, a bin width, a number of samples, and one or more dBs.
    //          That means that line.length() - 6 is the number of frequencies measured in that range.
    //          That number can be used in a for-loop to ensure that every dB is paired with a frequency without having to be reprogrammed.

    while (true) {
        cout << "Run!" << endl;
        nineMHz = exec(nineMHzCommand);
        twoGHz = exec(twoGHzCommand);
        fiveGHz = exec(fiveGHzCommand);

        nineMHz_sub = breakUp(nineMHz);
        twoGHz_sub = breakUp(twoGHz);
        fiveGHz_sub = breakUp(fiveGHz);

        nineMHz_processed = process(nineMHz_sub);
        twoGHz_processed = process(twoGHz_sub);
        fiveGHz_processed = process(fiveGHz_sub);

        std::sort(nineMHz_processed.begin(), nineMHz_processed.end());
        std::sort(twoGHz_processed.begin(), twoGHz_processed.end());
        std::sort(fiveGHz_processed.begin(), fiveGHz_processed.end());

        printout(nineMHz_processed);
        printout(twoGHz_processed);
        printout(fiveGHz_processed);
    }

	return 0;
}

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

string GetStdoutFromCommand(string cmd) {

    string data;
    FILE* stream;
    const int max_buffer = 256;
    char buffer[max_buffer];
    cmd.append(" 2>&1");

    stream = popen(cmd.c_str(), "r");

    if (stream) {
        while (!feof(stream))
            if (fgets(buffer, max_buffer, stream) != NULL) data.append(buffer);
        pclose(stream);
    }
    return data;
}


vector<vector<string>> breakUp(string originalText) {
    vector<vector<string>> container;
    vector<string> row;
    string substring = "";

    for (int i = 0; i < originalText.length(); i++) {
        if (originalText[i] == ',') {
            row.push_back(substring);
            substring = "";
        }
        else if (originalText[i] == ' ') {
            //Do nothing
        }
        else if (originalText[i] == '\n') {
            row.push_back(substring);
            container.push_back(row);
            row.clear();
        }
        else {
            substring = substring + originalText[i];
        }
    }
    return container;
}

vector<tuple<uint64_t, int>> process(vector<vector<string>> original) {
    //Each row has 11 columns
    //Date, time, Start Frequency, End Frequency, Bin width, number of samples, bin1, bin2, bin3, bin4, bin5
    //Number of bins = End - Start / Bin width

    vector<tuple<uint64_t, int>> container;
    tuple<uint64_t, int> entry;

    for (vector<string> row : original) {

        entry = make_tuple(stoull(row[2]), stoi(row[6]));
        container.push_back(entry);

        entry = make_tuple(stoull(row[2]) + stoull(row[4]), stoi(row[7]));
        container.push_back(entry);

        entry = make_tuple(stoull(row[2]) + 2 * stoull(row[4]), stoi(row[8]));
        container.push_back(entry);

        entry = make_tuple(stoull(row[2]) + 3 * stoull(row[4]), stoi(row[9]));
        container.push_back(entry);

        entry = make_tuple(stoull(row[2]) + 4 * stoull(row[4]), stoi(row[10]));
        container.push_back(entry);
    }
    return container;
}

void printout(vector<tuple<uint64_t, int>> current) {
    for (int i = 0; i < current.size(); i++) {
        cout << get<0>(current[i]) << " Hz: " << get<1>(current[i]) << endl;
    }
}