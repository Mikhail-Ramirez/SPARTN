#ifndef hsLS_h
#define hsLS_h

#include <string>

#include <vector>

#include <bits/stdc++.h>

std::string exec(const char*);

std::vector<std::vector<std::string>> breakUp(std::string);

std::vector<std::tuple<std::uint64_t, int>> process(std::vector<std::vector<std::string>>);

void printout(std::vector<std::tuple<std::uint64_t, int>>);

std::string GetStdoutFromCommand(std::string cmd);

std::vector<std::tuple<std::uint64_t, int>> sweep(std::string);

#endif