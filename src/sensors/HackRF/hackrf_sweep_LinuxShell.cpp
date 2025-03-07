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

    for (vector<string> row : nineMHz_sub) {
        for (string entry : row) {
            cout << entry << endl;
        }
        cout << endl;
    }

    for (vector<string> row : twoGHz_sub) {
        for (string entry : row) {
            cout << entry << endl;
        }
        cout << endl;
    }

    for (vector<string> row : fiveGHz_sub) {
        for (string entry : row) {
            cout << entry << endl;
        }
        cout << endl;
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