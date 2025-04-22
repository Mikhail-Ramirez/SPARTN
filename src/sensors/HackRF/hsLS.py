# Importing required module
from re import S
import subprocess

def exec(command):
    #print(subprocess.run(command.split(), stdout=subprocess.PIPE, stderr=subprocess.DEVNULL).stdout.decode('utf-8'))
    return subprocess.run(command.split(), stdout=subprocess.PIPE, stderr=subprocess.DEVNULL).stdout.decode('utf-8')

#getHackRFIDs
#This function checks for all HackRF devices connected and gets every HackRF's serial number.
def getHackRFIDs():
    ID_Command = "hackrf_info"

    Output_String = exec(ID_Command)
    
    snList = []
    line = ""

    for char in Output_String:
        if char == ' ':
            #Space detected.  This isn't what we want.
            line = ""

        elif char == '\n':
            #Check if  this is the line we want.
            if line[:4] == "0000":
                #This is what we want!
                snList.append(line)

            #Either way, erase the line.
            line = ""
        
        else:
            #This is just a character.
            line = line + char

    if len(snList) == 0:
        return "No HackRFs Detected!"

    else:
        return snList


def breakUp(originalText):
    #Pre-allocate the variables before the loop.
    container = []
    row = []
    substring = ""

    #Parse through the entire string.  Strings need to be seperated by commas and newlines, while everything else is added to the string.
    for symbol in originalText:
        if symbol == ',':
            #Break in entries found!  It's the same row, but new entry.
            row.append(substring);
            substring = ""

        elif substring == ',':
            #Do nothing since a comma was just found.
            True == True

        elif symbol == '\n':
            #End of line.  Add to row, then add row to container.
            row.append(substring)
            container.append(row.copy())
            row.clear()
            substring = ""
        
        else:
            #Otherwise, save the character to the string holding the entry.
            substring = substring + symbol

        #print(symbol)
        
    
    return container


def process(original):
    #Create the variables before the loop.
    container = []
    entry = []

    #Calculate the number of entries per line.  This allows for a variable number of entries per row to still be processed.
    entryCount = len(original[0]) - 6;

    #For every row in the original vector of vectors of strings...
    for row in original:

        #For every frequency measurement in the row, calculate the frequency each dB measurement was measured at, store both in a tuple, and append to container.
        for i in range(0, entryCount):
            entry = [int(row[2]) + i * int(row[4][:-3]), float(row[6+i])]
            container.append(entry.copy());
        
    
    return container;


def sweep(commandLine):
    freq_dB_raw = exec(commandLine)

    freq_dB_brokenUp = breakUp(freq_dB_raw)

    freq_dB_processed = process(freq_dB_brokenUp)
    
    freq_dB_processed.sort(key=lambda x: x[0])

    return freq_dB_processed;

def printout(freq_dB):
    for entry in freq_dB:
        print(entry[0], ", ", entry[1])

#####################################################Main
#This function demonstrates the use of the HackRF sweep code.
def main():

    print("Starting...  Press Ctrl-C to stop!")

    #First, get the list of HackRFs available.
    SN_List = getHackRFIDs()

    #Print out every serial number.
    for SN in SN_List:
        print("SN: ", SN)

    #These commands run the Linux Shell's hackrf_sweep command.
    #The flags are described as:
        #-f determines the frequency range (in MHz) to sweep non-inclusive.  -f 5:10 starts at 5 MHz and ends before 10 MHz.
        #-N determines the number of sweeps to do before the command ends.  Because we constantly call the command, leave -N 1.
        #-w is the frequency bin width, or "frequency resolution", in Hz.  That is essentially how big of steps it takes while sweeping.  The default is 1 MHz.
        #-l is the LVA gain in dB.  The default LVA gain in SDR++ is 40 dB, so I suggest 40 dB as well.
        #-g is the GVA gain in dB.  I read online that a recommended value is 8 dB, so I made it that here.
        #-d accepts the Serial Number of the HackRF to use.  The SN gets appended during the loop.
    #2>/dev/null is not a flag.  The hackrf_sweep command has debug output that spams the command line and 2>/dev/null suppresses it.
    nineMHzCommandStart = "hackrf_sweep -f 902:928 -N 1 -w 1000000 -l 40 -g 8 -d "
    twoGHzCommandStart = "hackrf_sweep -f 2400:2495 -N 1 -w 1000000 -l 40 -g 8 -d "
    fiveGHzCommandStart = "hackrf_sweep -f 5150:5895 -N 1 -w 1000000 -l 40 -g 8 -d "

    commandEnd = " 2>/dev/null"

    #Create the local variables to store the output of the command being run.

    nineMHz_data = []
    twoGHz_data = []
    fiveGHz_data = []

    print(nineMHzCommandStart + SN + commandEnd)

    #Main loop
    while True:
        print("Run!")

        for SN in SN_List:
            print("Running 9MHz with ", SN);
            nineMHz_data = sweep(nineMHzCommandStart + SN + commandEnd);
            print("Printing 9MHz with ", SN);
            printout(nineMHz_data)

        for SN in SN_List:
            print("Running 2GHz with ", SN);
            twoGHz_data = sweep(twoGHzCommandStart + SN + commandEnd);
            print("Printing 2GHz with ", SN);
            printout(twoGHz_data)

        for SN in SN_List:
            print("Running 5GHz with ", SN);
            fiveGHz_data = sweep(fiveGHzCommandStart + SN + commandEnd);
            print("Printing 5GHz with ", SN);
            printout(fiveGHz_data)
##########################################End Main






#If this is the file that is run, run the main function.
#This allows another file to call the functions in this file without this file actually running.
if __name__ == "__main__":
    main()