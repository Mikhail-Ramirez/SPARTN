#!/usr/bin/env python3
import numpy as np
import time
import sounddevice as sd
from scipy import signal

# Parameters
fs = 44100       # Sampling frequency in Hz
duration = 0.05  # Duration of the sound in seconds
f = 440          # Frequency of the square wave in Hz (A4 note)

# Time vector
t = np.linspace(0, duration, int(fs * duration), endpoint=False)

# Generate square wave using SciPy's square function
square_wave = signal.square(2 * np.pi * f * t)

# Infinite loop to play the sound repeatedly
while True:
    sd.play(square_wave, fs)
    # Optionally wait until the sound is finished playing
    # sd.wait() 
    time.sleep(0.1)

