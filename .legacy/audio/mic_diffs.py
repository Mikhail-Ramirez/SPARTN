#!/usr/bin/env python3
"""
TDOA Analysis Script for a Continuous Square/Sawtooth Wave

This script:
  1. Records audio from 3 microphones simultaneously for 30 seconds.
  2. Saves all raw samples to a CSV file ("recorded_samples.csv").
  3. Splits the recording into short windows (default 0.1 s) and computes the delay 
     (time difference of arrival, TDOA) between Mic4 (reference) and the other two mics using cross-correlation.
  4. Saves the computed TDOA data to a CSV file ("tdoa_delays.csv") and plots the delays over time.
  5. Computes and prints the average delay and percent error for Mic2 and Mic3 relative to Mic4.
  
Enjoy the script that listens for 30 seconds so you can finally judge whether your mics are "in sync"—or just wildly off-beat!
"""

import numpy as np
import sounddevice as sd
import matplotlib.pyplot as plt
from scipy.signal import correlate
import csv
import threading

# ========================
# Constants & Microphone Setup
# ========================
SAMPLE_RATE = 48000         # Hz
DURATION = 30               # seconds
CHANNELS = 1                # mono recording

# Microphone positions (example mapping: ALSA card numbers → physical positions)
MIC_POSITIONS = {
    4: np.array([0, 0]),    # Mic 4 (we’ll use this as our reference)
    3: np.array([0, 0])    # Mic 2
}

# The order in which mics will be recorded. (Feel free to adjust if needed.)
MIC_ORDER = [4, 3]

# Dictionary to hold the recordings
recordings = {}

# ========================
# Recording Functions
# ========================
def record_mic(mic_id):
    """Records audio from a single microphone."""
    global recordings
    print(f"🎤 Starting 30-second recording on Mic {mic_id} (device: hw:{mic_id},0)...")
    try:
        audio_data = sd.rec(int(DURATION * SAMPLE_RATE), samplerate=SAMPLE_RATE,
                            channels=CHANNELS, device=f"hw:{mic_id},0", dtype=np.float32)
        sd.wait()  # Wait for recording to finish
        recordings[mic_id] = audio_data.flatten()
        print(f"✅ Mic {mic_id} recorded (Max amplitude: {np.max(np.abs(audio_data)):.3f})")
    except Exception as e:
        print(f"❌ Error recording Mic {mic_id}: {e}")
        recordings[mic_id] = np.zeros(int(DURATION * SAMPLE_RATE))

def record_all_mics():
    """Starts simultaneous recording on all microphones."""
    threads = []
    for mic in MIC_ORDER:
        thread = threading.Thread(target=record_mic, args=(mic,))
        thread.start()
        threads.append(thread)
    for thread in threads:
        thread.join()  # Wait for all threads to finish
    # Return recordings in the order defined by MIC_ORDER
    return [recordings[mic] for mic in MIC_ORDER]

# ========================
# CSV Saving Functions
# ========================
def save_samples_to_csv(recordings_list, filename="recorded_samples.csv"):
    """
    Saves the full recordings from all microphones to a CSV file.
    CSV columns: Time, Mic4, Mic2, Mic3.
    Warning: This file will have SAMPLE_RATE * DURATION rows.
    """
    num_samples = len(recordings_list[0])
    times = np.arange(num_samples) / SAMPLE_RATE
    header = ["Time (s)"] + [f"Mic {mic}" for mic in MIC_ORDER]
    
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(header)
        for i in range(num_samples):
            row = [times[i]]
            for rec in recordings_list:
                row.append(rec[i])
            writer.writerow(row)
    print(f"✅ Recorded samples saved to {filename}")

def save_delays_to_csv(times, delays_mic2, delays_mic3, filename="tdoa_delays.csv"):
    """
    Saves the computed time delays (TDOA) to a CSV file.
    Columns: Time, Delay Mic2 (s), Delay Mic3 (s)
    """
    header = ["Time (s)", "Delay Mic2 (s)", "Delay Mic3 (s)"]
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(header)
        for t, d3 in zip(times, delays_mic2, delays_mic3):
            writer.writerow([t, d3])
    print(f"✅ Delay data saved to {filename}")

# ========================
# TDOA Analysis Functions
# ========================
def analyze_tdoa(recordings_list, window_duration=0.1):
    """
    Splits the recordings into windows (default 0.1 s) and computes the time delay for Mic2 and Mic3
    relative to Mic4 (assumed reference) using cross-correlation.
    
    Returns:
      times: Array of mid-window times (in seconds).
      delays_mic2: Array of computed delays for Mic2 (in seconds).
      delays_mic3: Array of computed delays for Mic3 (in seconds).
    """
    window_size = int(window_duration * SAMPLE_RATE)
    num_windows = len(recordings_list[0]) // window_size

    times = []
    delays_mic3 = []

    # Define reference and other mic signals
    ref_signal = recordings_list[0]      # Mic4 as reference
    mic3_signal = recordings_list[1]

    print(f"🔍 Analyzing TDOA in {num_windows} windows (each {window_duration} s)...")

    for i in range(num_windows):
        start = i * window_size
        end = start + window_size

        # Get the segments for each mic
        ref_seg = ref_signal[start:end]
        mic3_seg = mic3_signal[start:end]

        # Cross-correlation between Mic3 and reference:
        corr3 = correlate(mic3_seg, ref_seg, mode='full')
        lags3 = np.arange(-len(ref_seg) + 1, len(ref_seg))
        peak_index3 = np.argmax(np.abs(corr3))
        delay3 = lags3[peak_index3] / SAMPLE_RATE

        mid_time = (start + end) / (2 * SAMPLE_RATE)
        times.append(mid_time)
        delays_mic3.append(delay3)

    return np.array(times), np.array(delays_mic3)

def plot_delays(times, delays_mic3, filename="tdoa_delays.png"):
    """Plots the computed time delays over the recording duration."""
    plt.figure(figsize=(10, 6))
    plt.plot(times, delays_mic3, label="Delay Mic2 relative to Mic4", marker='o', linestyle='-')
    plt.xlabel("Time (s)")
    plt.ylabel("Delay (s)")
    plt.title("Time Difference of Arrival (TDOA) for Mic2 & Mic3 (relative to Mic4)")
    plt.legend()
    plt.grid(True)
    plt.savefig(filename)
    print(f"✅ Delay plot saved as {filename}")
    plt.show()

def compute_average_and_percent_error(delays):
    """
    Computes the average delay and percent error (std. dev / |mean| * 100)
    for an array of delay values.
    """
    avg_delay = np.mean(delays)
    std_delay = np.std(delays)
    percent_error = (std_delay / abs(avg_delay)) * 100 if avg_delay != 0 else 0
    return avg_delay, percent_error

# ========================
# Main Execution
# ========================
def main():
    print("🎧 Starting a 30-second recording session for TDOA analysis...")
    recordings_list = record_all_mics()
    
    # Save all the raw samples to CSV (this file will be huge, but it’s all yours!)
    save_samples_to_csv(recordings_list, filename="recorded_samples.csv")
    
    # Analyze the time differences of arrival using a sliding window approach
    window_duration = 0.1  # seconds per analysis window (feel free to adjust)
    times, delays_mic3 = analyze_tdoa(recordings_list, window_duration=window_duration)
    
    # Save the computed delay values to CSV
    save_delays_to_csv(times, delays_mic3, filename="tdoa_delays.csv")
    
    # Plot the delays over time
    plot_delays(times, delays_mic3, filename="tdoa_delays.png")
    
    # Compute and print average delay and percent error for each non-reference mic
    avg_delay3, percent_error3 = compute_average_and_percent_error(delays_mic3)
    
    print("\n===== TDOA Analysis Results =====")
    print(f"Mic3 Delay (relative to Mic4): Average = {avg_delay3:.6f} s, Percent Error = {percent_error3:.2f}%")
    print("=================================")

if __name__ == "__main__":
    main()

