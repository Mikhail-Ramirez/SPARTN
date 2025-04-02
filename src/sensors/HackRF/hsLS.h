//hsLS.h
//Made by Dawson Hampton, dth0024@uah.edu
//2025-3-26

//This file is the header file for the sweep() function for the SPARTN Drone Detection system.

#ifndef hsLS_h
#define hsLS_h

#include <string>
#include <vector>
#include <bits/stdc++.h>

//This function accepts the total output of hackrf_sweep from the Linux shell and parses each row into its own vector of strings where
// every string is an entry in the row.
//It then compiles every row inside the vectors of strings into a single vector of vectors of strings and returns it.
std::vector<std::vector<std::string>> breakUp(std::string &);

//This function processes the vector of vectors of strings made by breakUp() into a vector of frequency-decibel tuples.
//This creates a single vector where a frequency can be looked up and its corresponding dB measurement can be found.
std::vector<std::tuple<std::uint64_t, int>> process(std::vector<std::vector<std::string>> &);

//This is a debugging funtion that prints out the vector of frequency-dB tuples.
void printout(std::vector<std::tuple<std::uint64_t, int>> &);

//sweep() accepts a string of a command to run (preferably a hackrf_sweep command), parses all the data, and returns a vector of frequency-dB tuples.
std::vector<std::tuple<std::uint64_t, int>> sweep(std::string);

std::vector<std::string> getHackrfIDs();

//Taken from https://stackoverflow.com/questions/478898/how-do-i-execute-a-command-and-get-the-output-of-the-command-within-c-using-po
//This command accepts a const char* command, feeds it through the Linux shell, and returns what would have been outputted to the Linux shell.
std::string exec(const char*);

#endif