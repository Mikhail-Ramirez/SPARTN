//hackrf_sweep_LinuxShell.cpp
//Made by Dawson Hampton, dth0024@uah.edu
//2025-3-26

//This program demonstrates the sweep() function stored in hsLS.cpp.

#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>

#include "hsLS.h"

using namespace std;

int main() {
	cout << "Starting...  Press Ctrl-C to exit!" << endl;

    //Create the command strings to be executed.

    //First, get the Serial Numbers of the HackRFs.
    vector<string> SN_List = getHackrfIDs();

    for (int i = 0; i < SN_List.size(); i++) {
        cout << "SN: " << SN_List[i] << endl;
    }

    //These commands run the Linux Shell's hackrf_sweep command.
    //The flags are described as:
        //-f determines the frequency range (in MHz) to sweep non-inclusive.  -f 5:10 starts at 5 MHz and ends before 10 MHz.
        //-N determines the number of sweeps to do before the command ends.  Because we constantly call the command, leave -N 1.
        //-w is the frequency bin width, or "frequency resolution", in Hz.  That is essentially how big of steps it takes while sweeping.  The default is 1 MHz.
        //-l is the LVA gain in dB.  The default LVA gain in SDR++ is 40 dB, so I suggest 40 dB as well.
        //-g is the GVA gain in dB.  I read online that a recommended value is 8 dB, so I made it that here.
        //-d accepts the Serial Number of the HackRF to use.  The SN gets appended during the loop.
    //2>/dev/null is not a flag.  The hackrf_sweep command has debug output that spams the command line and 2>/dev/null suppresses it.
    string nineMHzCommandStart = "hackrf_sweep -f 902:928 -N 1 -w 1000000 -l 40 -g 8 -d ";
    string twoGHzCommandStart = "hackrf_sweep -f 2400:2495 -N 1 -w 1000000 -l 40 -g 8 -d ";
    string fiveGHzCommandStart = "hackrf_sweep -f 5150:5895 -N 1 -w 1000000 -l 40 -g 8 -d ";

    string commandEnd = " 2>/dev/null";


    //Create the local variables to store the output of the command being run.

    vector<tuple<uint64_t, int>> nineMHz_data;
    vector<tuple<uint64_t, int>> twoGHz_data;
    vector<tuple<uint64_t, int>> fiveGHz_data;

    while (true) {
        cout << "Run!" << endl;

        for (int i = 0; i < SN_List.size(); i++) {
            cout << "Running 9MHz with " + SN_List[i] << endl;
            nineMHz_data = sweep(nineMHzCommandStart + SN_List[i] + commandEnd);
            cout << "Printing 9MHz with " + SN_List[i] << endl;
            printout(nineMHz_data);
        }
        for (int i = 0; i < SN_List.size(); i++) {
            cout << "Running 2.4GHz with " + SN_List[i] << endl;
            twoGHz_data = sweep(twoGHzCommandStart + SN_List[i] + commandEnd);
            cout << "Printing 2.4GHz with " + SN_List[i] << endl;
            printout(twoGHz_data);
        }
        for (int i = 0; i < SN_List.size(); i++) {
            cout << "Running 5GHz with " + SN_List[i] << endl;
            fiveGHz_data = sweep(fiveGHzCommandStart + SN_List[i] + commandEnd);
            cout << "Printing 5GHz with " + SN_List[i] << endl;
            printout(fiveGHz_data);
        }
    }

	return 0;
}