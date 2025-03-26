#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "hsLS.h"

using namespace std;

int main() {
	cout << "Starting...  Press Ctrl-C to exit!" << endl;

    //Create the command strings to be executed.
    //TODO: Explain what these commands mean.

    string nineMHzCommand = "hackrf_sweep -f 902:928 -N 1 -w 1000000 -l 40 -g 8 2>/dev/null";
    string twoGHzCommand = "hackrf_sweep -f 2400:2495 -N 1 -w 1000000 -l 40 -g 8 2>/dev/null";
    string fiveGHzCommand = "hackrf_sweep -f 5150:5895 -N 1 -w 1000000 -l 40 -g 8 2>/dev/null";


    //Create the local variables to store the output of the command being run.

    vector<tuple<uint64_t, int>> nineMHz_data;
    vector<tuple<uint64_t, int>> twoGHz_data;
    vector<tuple<uint64_t, int>> fiveGHz_data;

    //TODO: Add comments.  Lots and lots of comments.

    while (true) {
        cout << "Run!" << endl;

        nineMHz_data = sweep(nineMHzCommand);
        twoGHz_data = sweep(twoGHzCommand);
        fiveGHz_data = sweep(fiveGHzCommand);


        printout(nineMHz_data);
        printout(twoGHz_data);
        printout(fiveGHz_data);
    }

	return 0;
}