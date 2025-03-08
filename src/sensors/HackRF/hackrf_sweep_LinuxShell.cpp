#include <iostream>
#include <string>
#include <stdlib.h>

#include <vector>

#include <bits/stdc++.h>

#include <algorithm>

#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>

using namespace std;

std::string exec(const char* cmd);
vector<vector<string>> breakUp(string);
vector<tuple<uint64_t, int>> process(vector<vector<string>>);

int main() {
	cout << "Starting..." << endl;

	//string Test = exec("hackrf_sweep -f 2400:2490 -N 1");

    string nineMHz = exec("hackrf_sweep -f 902:928 -N 1");
    string twoGHz = exec("hackrf_sweep -f 2400:2495 -N 1");
    string fiveGHz = exec("hackrf_sweep -f 5150:5895 -N 1");

    // Takes only space separated C++ strings.
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