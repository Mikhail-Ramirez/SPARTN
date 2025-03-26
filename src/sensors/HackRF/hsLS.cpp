#include "hsLS.h"

#include <iostream>
#include <string>
#include <stdlib.h>

#include <vector>

#include <bits/stdc++.h>

#include <algorithm>

#include <cstdio>
#include <memory>

std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<std::FILE, decltype(&pclose)> pipe(popen(cmd, "r"), pclose);
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}

std::string GetStdoutFromCommand(std::string cmd) {

    std::string data;
    std::FILE* stream;
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


std::vector<std::vector<std::string>> breakUp(std::string originalText) {
    std::vector<std::vector<std::string>> container;
    std::vector<std::string> row;
    std::string substring = "";

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

std::vector<std::tuple<std::uint64_t, int>> process(std::vector<std::vector<std::string>> original) {
    //Each row has 11 columns
    //Date, time, Start Frequency, End Frequency, Bin width, number of samples, bin1, bin2, bin3, bin4, bin5
    //Number of bins = End - Start / Bin width

    //TODO: Rework how the process function processes data, preferibly so that it can parse it regardless of how many entries there are in a line.
    //          Each line has a date, a time, a starting freq, and end freq, a bin width, a number of samples, and one or more dBs.
    //          That means that line.length() - 6 is the number of frequencies measured in that range.
    //          That number can be used in a for-loop to ensure that every dB is paired with a frequency without having to be reprogrammed.

    std::vector<std::tuple<std::uint64_t, int>> container;
    std::tuple<std::uint64_t, int> entry;

    for (std::vector<std::string> row : original) {

        entry = std::make_tuple(stoull(row[2]), stoi(row[6]));
        container.push_back(entry);

        entry = std::make_tuple(stoull(row[2]) + stoull(row[4]), stoi(row[7]));
        container.push_back(entry);

        entry = std::make_tuple(stoull(row[2]) + 2 * stoull(row[4]), stoi(row[8]));
        container.push_back(entry);

        entry = std::make_tuple(stoull(row[2]) + 3 * stoull(row[4]), stoi(row[9]));
        container.push_back(entry);

        entry = std::make_tuple(stoull(row[2]) + 4 * stoull(row[4]), stoi(row[10]));
        container.push_back(entry);
    }
    return container;
}

void printout(std::vector<std::tuple<std::uint64_t, int>> current) {
    for (int i = 0; i < current.size(); i++) {
        std::cout << std::get<0>(current[i]) << " Hz: " << std::get<1>(current[i]) << std::endl;
    }
}

std::vector<std::tuple<std::uint64_t, int>> sweep(std::string command) {
    std::string freq_dB_raw = exec(command.c_str());

    std::vector<std::vector<std::string>> freq_dB_brokenUp = breakUp(freq_dB_raw);

    std::vector<std::tuple<std::uint64_t, int>> freq_dB_processed = process(freq_dB_brokenUp);

    std::sort(freq_dB_processed.begin(), freq_dB_processed.end());

    return freq_dB_processed;
}