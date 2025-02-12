#!/usr/bin/env python3
"""
TDOA Analysis Script for a Continuous Square/Sawtooth Wave (2-Microphone Version)

This script:
  1. Records audio from 2 microphones simultaneously for 30 seconds.
  2. Saves all raw samples to a CSV file ("recorded_samples.csv").
  3. Splits the recording into short windows (default 0.1 s) and computes the delay
     (time difference of arrival, TDOA) between Mic4 (reference) and Mic3 using cross-correlation.
  4. Saves the computed TDOA data to a CSV file ("tdoa_delays.csv") and plots the delay over time.
  5. Computes and prints the average delay and percent error for Mic3 relative to Mic4.

Listen up—this script records 30 seconds so you can see if your mics are in sync or if one’s dancing to its own beat!
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

# In this 2-microphone setup, we'll use Mic4 as the reference and Mic3 as the secondary.
MIC_ORDER = [4, 3]  # first is reference (Mic4), second is the other mic (Mic3)

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
    CSV columns: Time, Mic4, Mic3.
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

def save_delays_to_csv(times, delays, filename="tdoa_delays.csv"):
    """
    Saves the computed time delays (TDOA) to a CSV file.
    Columns: Time (s), Delay Mic3 (s)
    """
    header = ["Time (s)", "Delay Mic3 (s)"]
    with open(filename, 'w', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow(header)
        for t, d in zip(times, delays):
            writer.writerow([t, d])
    print(f"✅ Delay data saved to {filename}")

# ========================
# TDOA Analysis Functions
# ========================
def analyze_tdoa(recordings_list, window_duration=0.1):
    """
    Splits the recordings into windows (default 0.1 s) and computes the time delay for Mic3
    relative to Mic4 (assumed reference) using cross-correlation.

    Returns:
      times: Array of mid-window times (in seconds).
      delays: Array of computed delays for Mic3 (in seconds).
    """
    window_size = int(window_duration * SAMPLE_RATE)
    num_windows = len(recordings_list[0]) // window_size

    times = []
    delays = []

    # Define reference and secondary mic signals
    ref_signal = recordings_list[0]   # Mic4 as reference
    mic_signal = recordings_list[1]   # Mic3

    print(f"🔍 Analyzing TDOA in {num_windows} windows (each {window_duration} s)...")

    for i in range(num_windows):
        start = i * window_size
        end = start + window_size

        # Get the segments for each mic
        ref_seg = ref_signal[start:end]
        mic_seg = mic_signal[start:end]

        # Cross-correlation between Mic3 and reference (Mic4):
        corr = correlate(mic_seg, ref_seg, mode='full')
        lags = np.arange(-len(ref_seg) + 1, len(ref_seg))
        peak_index = np.argmax(np.abs(corr))
        delay = lags[peak_index] / SAMPLE_RATE

        mid_time = (start + end) / (2 * SAMPLE_RATE)
        times.append(mid_time)
        if (delay < 0.09):
            delays.append(delay)

    return np.array(times), np.array(delays)

def plot_delays(times, delays, filename="tdoa_delays.png"):
    """Plots the computed time delays over the recording duration."""
    plt.figure(figsize=(10, 6))
    plt.plot(times, delays, label="Delay Mic3 relative to Mic4", marker='o', linestyle='-')
    plt.xlabel("Time (s)")
    plt.ylabel("Delay (s)")
    plt.title("Time Difference of Arrival (TDOA) for Mic3 (relative to Mic4)")
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
    print("🎧 Starting a 30-second recording session for TDOA analysis with 2 mics...")
    recordings_list = record_all_mics()

    # Save all the raw samples to CSV (brace yourself—the file may be huge!)
    save_samples_to_csv(recordings_list, filename="recorded_samples.csv")

    # Analyze the time difference of arrival using a sliding window approach
    window_duration = 0.1  # seconds per analysis window (tweak as desired)
    times, delays = analyze_tdoa(recordings_list, window_duration=window_duration)

    # Save the computed delay values to CSV
    save_delays_to_csv(times, delays, filename="tdoa_delays.csv")

    # Plot the delays over time
    plot_delays(times, delays, filename="tdoa_delays.png")

    # Compute and print average delay and percent error for Mic3 relative to Mic4
    avg_delay, percent_error = compute_average_and_percent_error(delays)
    print("\n===== TDOA Analysis Results =====")
    print(f"Mic3 Delay (relative to Mic4): Average = {avg_delay:.6f} s, Percent Error = {percent_error:.2f}%")
    print("=================================")

if __name__ == "__main__":
    main()

