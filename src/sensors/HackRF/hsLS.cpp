//hsLS.cpp
//Made by Dawson Hampton, dth0024@uah.edu
//2025-3-26

//This file stores the sweep() function for the SPARTN Drone Detection system.

#include "hsLS.h"

#include <iostream>
#include <string>
#include <stdlib.h>

#include <vector>
#include <bits/stdc++.h>
#include <algorithm>
#include <cstdio>
#include <memory>

//This function accepts the total output of hackrf_sweep from the Linux shell and parses each row into its own vector of strings where
// every string is an entry in the row.
//It then compiles every row inside the vectors of strings into a single vector of vectors of strings and returns it.
std::vector<std::vector<std::string>> breakUp(std::string & originalText) {

    //Pre-allocate the variables before the loop.
    std::vector<std::vector<std::string>> container;
    std::vector<std::string> row;
    std::string substring = "";

    //Parse through the entire string.  Strings need to be seperated by commas and newlines, while everything else is added to the string.
    for (int i = 0; i < originalText.length(); i++) {
        if (originalText[i] == ',') {
            //Break in entries found!  It's the same row, but new entry.
            row.push_back(substring);
            substring = "";
        }
        else if (originalText[i] == ' ') {
            //Do nothing since a comma was just found.
        }
        else if (originalText[i] == '\n') {
            //End of line.  Add to row, then add row to container.
            row.push_back(substring);
            container.push_back(row);
            row.clear();
            substring = "";
        }
        else {
            //Otherwise, save the character to the string holding the entry.
            substring = substring + originalText[i];
        }
    }
    return container;
}


//This function processes the vector of vectors of strings made by breakUp() into a vector of frequency-decibel tuples.
//This creates a single vector where a frequency can be looked up and its corresponding dB measurement can be found.
std::vector<std::tuple<std::uint64_t, int>> process(std::vector<std::vector<std::string>> & original) {
    //Create the variables before the loop.
    std::vector<std::tuple<std::uint64_t, int>> container;
    std::tuple<std::uint64_t, int> entry;

    //Calculate the number of entries per line.  This allows for a variable number of entries per row to still be processed.
    int entryCount = original[0].size() - 6;

    //For every row in the original vector of vectors of strings...
    for (std::vector<std::string> row : original) {

        //For every frequency measurement in the row, calculate the frequency each dB measurement was measured at, store both in a tuple, and append to container.
        for (int i = 0; i < entryCount; i++) {
            entry = std::make_tuple(stoull(row[2]) + i* stoull(row[4]), stoi(row[6 + i]));
            container.push_back(entry);
        }
    }
    return container;
}

//This is a debugging funtion that prints out the vector of frequency-dB tuples.
void printout(std::vector<std::tuple<std::uint64_t, int>> & current) {
    for (int i = 0; i < current.size(); i++) {
        std::cout << std::get<0>(current[i]) << " Hz: " << std::get<1>(current[i]) << std::endl;
    }
}

//sweep() accepts a string of a command to run (preferably a hackrf_sweep command), parses all the data, and returns a vector of frequency-dB tuples.
std::vector<std::tuple<std::uint64_t, int>> sweep(std::string command) {
    std::string freq_dB_raw = exec(command.c_str());

    std::vector<std::vector<std::string>> freq_dB_brokenUp = breakUp(freq_dB_raw);

    std::vector<std::tuple<std::uint64_t, int>> freq_dB_processed = process(freq_dB_brokenUp);
    
    std::sort(freq_dB_processed.begin(), freq_dB_processed.end());

    return freq_dB_processed;
}

//This function runs the hackrf_info command and parses the output to make a list of serial numbers.
std::vector<std::string> getHackrfIDs() {
    std::cout << "Running getHackrfIDs" << std::endl;
    std::string infoCommand = "hackrf_info";
    std::string unparsedData = exec(infoCommand.c_str());

    uint unparsedLength = unparsedData.length();

    std::vector<std::string> SN_List;
    std::string line = "";

    for (int i = 0; i < unparsedLength; i++) {
        if (unparsedData[i] == ' ') {
            //Space detected, this isn't what we want.
            line.erase();
        }
        else if (unparsedData[i] == '\n') {
            //Check if the line is what we want.
            if (line[0] == '0' && line[1] == '0' && line[2] == '0' && line[3] == '0') {
                //This is what we want!
                SN_List.push_back(line);
            }
            line.erase();
        }
        else {
            //This is just a character.
            line = line + unparsedData[i];
        }
    }

    std::sort(SN_List.begin(), SN_List.end());

    return SN_List;
}


//********************Helper command**********************//
//Taken from https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po

//This command accepts a const char* command, feeds it through the Linux shell, and returns what would have been outputted to the Linux shell.
std::string exec(const char* cmd) {
    std::array<char, 128> buffer;
    std::string result;
    std::unique_ptr<std::FILE, void(*)(FILE*)> pipe(popen(cmd, "r"), [](FILE* f) -> void {
            // wrapper to ignore the return value from pclose() is needed with newer versions of gnu g++
            std::ignore = pclose(f);
        }
    );
    if (!pipe) {
        throw std::runtime_error("popen() failed!");
    }
    while (fgets(buffer.data(), static_cast<int>(buffer.size()), pipe.get()) != nullptr) {
        result += buffer.data();
    }
    return result;
}