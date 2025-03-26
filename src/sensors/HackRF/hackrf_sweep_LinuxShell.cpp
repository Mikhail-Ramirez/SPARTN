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
void compare(vector<tuple<uint64_t, int>>, vector<tuple<uint64_t, int>>);
string GetStdoutFromCommand(string cmd);

const int TOLERANCE = 100;

int main() {
	cout << "Starting...  Press Ctrl-C to exit!" << endl;

	//string Test = exec("hackrf_sweep -f 2400:2490 -N 1");

    string nineMHz = exec("hackrf_sweep -f 902:928 -N 1");
    string twoGHz = exec("hackrf_sweep -f 2400:2495 -N 1");
    string fiveGHz = exec("hackrf_sweep -f 5150:5895 -N 1");

    vector<vector<string>> nineMHz_sub;
    vector<vector<string>> twoGHz_sub;
    vector<vector<string>> fiveGHz_sub;

    vector<tuple<uint64_t, int>> nineMHz_processed;
    vector<tuple<uint64_t, int>> twoGHz_processed;
    vector<tuple<uint64_t, int>> fiveGHz_processed;

    vector<tuple<uint64_t, int>> nineMHz_processed_old;
    vector<tuple<uint64_t, int>> twoGHz_processed_old;
    vector<tuple<uint64_t, int>> fiveGHz_processed_old;

    // Takes only space separated C++ strings.
    /*
    vector<vector<string>> nineMHz_sub = breakUp(nineMHz);
    vector<vector<string>> twoGHz_sub = breakUp(twoGHz);
    vector<vector<string>> fiveGHz_sub = breakUp(fiveGHz);

    vector<tuple<uint64_t, int>> nineMHz_processed = process(nineMHz_sub);
    vector<tuple<uint64_t, int>> twoGHz_processed = process(twoGHz_sub);
    vector<tuple<uint64_t, int>> fiveGHz_processed = process(fiveGHz_sub);

    //cout << "Processed!" << endl;

    for (tuple<uint64_t, int> row : nineMHz_processed) {
        cout << get<0>(row) << ", " << get<1>(row) << endl;
    }

    for (tuple<uint64_t, int> row : twoGHz_processed) {
        cout << get<0>(row) << ", " << get<1>(row) << endl;
    }

    for (tuple<uint64_t, int> row : fiveGHz_processed) {
        cout << get<0>(row) << ", " << get<1>(row) << endl;
    }
    */

    nineMHz_sub = breakUp(nineMHz);
    twoGHz_sub = breakUp(twoGHz);
    fiveGHz_sub = breakUp(fiveGHz);

    nineMHz_processed_old = process(nineMHz_sub);
    twoGHz_processed_old = process(twoGHz_sub);
    fiveGHz_processed_old = process(fiveGHz_sub);

    while (true) {
        cout << "Run!" << endl;
        //nineMHz = exec("hackrf_sweep -f 902:928 -N 1");
        //twoGHz = exec("hackrf_sweep -f 2400:2495 -N 1");
        //fiveGHz = exec("hackrf_sweep -f 5150:5895 -N 1");
        nineMHz = exec("hackrf_sweep -f 914:918 -N 1");

        nineMHz_sub = breakUp(nineMHz);
        //twoGHz_sub = breakUp(twoGHz);
        //fiveGHz_sub = breakUp(fiveGHz);

        nineMHz_processed = process(nineMHz_sub);
        //twoGHz_processed = process(twoGHz_sub);
        //fiveGHz_processed = process(fiveGHz_sub);

        compare(nineMHz_processed, nineMHz_processed_old);
        //compare(twoGHz_processed, twoGHz_processed_old);
        //compare(fiveGHz_processed, fiveGHz_processed_old);

        nineMHz_processed_old = nineMHz_processed;
        //twoGHz_processed_old = twoGHz_processed;
        //fiveGHz_processed_old = fiveGHz_processed;
    }


	//cout << "Test = " << subs[0] << endl;

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

        entry = make_tuple(stoull(row[2]) + stoull(row[4]), stoi(row[8]));
        container.push_back(entry);

        entry = make_tuple(stoull(row[2]) + 3 * stoull(row[4]), stoi(row[9]));
        container.push_back(entry);
    }
    return container;
}

void compare(vector<tuple<uint64_t, int>> current, vector<tuple<uint64_t, int>> old) {
    for (int i = 0; i < current.size(); i++) {
        /*if (get<1>(current[i]) < -100 || get<1>(current[i]) > 100 || get<1>(old[i]) < -100 || get<1>(old[i]) > 100) {
            //Skip this.  It is likely an error.
        }
        else if (get<1>(current[i]) < get<1>(old[i]) - TOLERANCE || get<1>(current[i]) > get<1>(old[i]) + TOLERANCE) {
            cout << "Detected change at " << get<0>(current[i]) << ": From " << get<1>(old[i]) << " to " << get<1>(current[i]) << endl;
        }*/
        cout << get<0>(current[i]) << " Hz: " << get<1>(current[i]) << endl;
    }
}