import numpy as np
import sounddevice as sd
from scipy.signal import correlate
import threading
import time
import csv
import os

# CONSTANTS
SAMPLE_RATE = 48000      # Hz
DURATION = 1             # seconds per iteration
CHANNELS = 1             # Mono Recording
SPEED_OF_SOUND = 343     # m/s

# Verified microphone positions (mic number → [x, y] in meters)
MIC_POSITIONS = {
    3: np.array([0, 0]),              # e.g., Mic 2
    2: np.array([3.6576, 3.6576]),     # e.g., Mic 3
    4: np.array([7.3152, 0])           # e.g., Mic 4
}

# Default recording order (order of mic IDs)
MIC_ORDER = [4, 2, 3]

# Storage for recorded audio
recordings = {}

CSV_FILE = "sound_source_log.csv"

def write_csv_header_if_needed():
    if not os.path.exists(CSV_FILE) or os.path.getsize(CSV_FILE) == 0:
        with open(CSV_FILE, 'w', newline='') as csvfile:
            writer = csv.writer(csvfile)
            writer.writerow(["timestamp", "reference_mic", "mic_order", "estimated_x", "estimated_y", "r1", "r2"])

def log_measurement(timestamp, reference_mic, mic_order, estimated_position, r1, r2):
    with open(CSV_FILE, 'a', newline='') as csvfile:
        writer = csv.writer(csvfile)
        writer.writerow([timestamp, reference_mic, "-".join(str(m) for m in mic_order),
                         estimated_position[0], estimated_position[1], r1, r2])

def record_mic(mic_id):
    global recordings
    print(f"Starting recording on Mic {mic_id} (hw:{mic_id},0)")
    try:
        audio_data = sd.rec(int(DURATION * SAMPLE_RATE),
                            samplerate=SAMPLE_RATE,
                            channels=CHANNELS,
                            device=f"hw:{mic_id},0",
                            dtype=np.float32)
        sd.wait()
        recordings[mic_id] = audio_data.flatten()
        print(f"Mic {mic_id} recorded. Max Amplitude: {np.max(np.abs(audio_data)):.3f}")
    except Exception as e:
        print(f"Error recording Mic {mic_id}: {e}")
        recordings[mic_id] = np.zeros(int(DURATION * SAMPLE_RATE))

def record_audio():
    """Starts recording for all microphones simultaneously."""
    global recordings
    print(f"\nRecording from all microphones simultaneously for {DURATION} second(s)...")
    threads = []
    recordings.clear()  # clear previous recordings
    for mic in MIC_ORDER:
        thread = threading.Thread(target=record_mic, args=(mic,))
        thread.start()
        threads.append(thread)
    for thread in threads:
        thread.join()
    # Return recordings in the default MIC_ORDER order
    return [recordings[mic] for mic in MIC_ORDER]

def determine_reference_mic_cc(recordings_list):
    """
    Determines the reference mic by computing cross correlations for each pair
    using the entire 1-second clip. For each candidate mic, the lags (i.e. the delays of 
    other mics relative to it) are summed. The mic with the highest total lag is chosen
    as the reference (since a positive lag indicates that the candidate received the sound earlier).
    """
    print("Determining Reference Mic based on cross-correlation over the entire clip...")
    scores = {}
    # recordings_list is assumed to be in the same order as MIC_ORDER
    for i, mic in enumerate(MIC_ORDER):
        score = 0.0
        for j, other_mic in enumerate(MIC_ORDER):
            if i == j:
                continue
            ref_signal = recordings_list[i]
            other_signal = recordings_list[j]
            correlation = correlate(ref_signal, other_signal, mode='full')
            lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
            peak_index = np.argmax(np.abs(correlation))
            lag = lags[peak_index]
            score += lag
            print(f"Cross-correlation: Mic {mic} vs Mic {other_mic}: lag = {lag:.6f} s")
        scores[mic] = score
        print(f"Total score for Mic {mic}: {score:.6f} s")
    reference_mic = max(scores, key=scores.get)
    print(f"Reference mic chosen (based on cross-correlation): Mic {reference_mic}")
    # Reorder the mic list so that the reference mic is first
    reordered_mics = [reference_mic] + [mic for mic in MIC_ORDER if mic != reference_mic]
    return reference_mic, reordered_mics

def cross_correlate(recordings_ordered, reordered_mics):
    """
    Computes time delays between the chosen reference mic (first in recordings_ordered)
    and the other microphones using cross-correlation.
    """
    print("Performing cross-correlation to compute time lags relative to reference...")
    ref_signal = recordings_ordered[0]
    time_lags = {}
    for i in range(1, len(recordings_ordered)):
        mic = reordered_mics[i]
        mic_signal = recordings_ordered[i]
        correlation = correlate(ref_signal, mic_signal, mode='full')
        lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE
        peak_index = np.argmax(np.abs(correlation))
        time_lag = lags[peak_index]
        time_lags[mic] = time_lag
        print(f"Time lag between Ref Mic (Mic {reordered_mics[0]}) and Mic {mic}: {time_lag:.6f} s")
    return time_lags

def localize_source(time_lags, reordered_mics):
    """
    Uses the computed time lags (converted to distances) to perform trilateration
    and estimate the sound source's (x, y) location.
    """
    print("Performing trilateration to locate sound source...")
    if len(reordered_mics) < 3:
        print("Not enough microphones for trilateration.")
        return np.array([None, None]), 0, 0
    # Convert time lags to distances (r = lag * SPEED_OF_SOUND)
    r1 = time_lags.get(reordered_mics[1], 0) * SPEED_OF_SOUND
    r2 = time_lags.get(reordered_mics[2], 0) * SPEED_OF_SOUND
    r3 = 0  # Reference mic has zero lag by definition
    print(f"Distances (meters): r1={r1:.3f}, r2={r2:.3f}")
    # Get tower positions based on reordered mic list
    tower1 = MIC_POSITIONS[reordered_mics[0]]
    tower2 = MIC_POSITIONS[reordered_mics[1]]
    tower3 = MIC_POSITIONS[reordered_mics[2]]
    # Trilateration equations:
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)
    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])
    try:
        estimated_position = np.linalg.solve(A_matrix, b_vector)
        print(f"Estimated position: x = {estimated_position[0]:.3f}, y = {estimated_position[1]:.3f}")
    except np.linalg.LinAlgError:
        print("Trilateration failed due to singular matrix.")
        estimated_position = np.array([None, None])
    return estimated_position, r1, r2

def main():
    write_csv_header_if_needed()
    print("Starting live trilateration prediction. Press Ctrl+C to stop.")
    try:
        while True:
            # Record 1 second of audio from all microphones
            recordings_list = record_audio()
            # Determine the reference mic based on whole-clip cross-correlation
            reference_mic, reordered_mics = determine_reference_mic_cc(recordings_list)
            # Reorder the recordings to match the new mic order
            recordings_ordered = [recordings_list[MIC_ORDER.index(mic)] for mic in reordered_mics]
            # Compute time lags (relative to the chosen reference mic)
            time_lags = cross_correlate(recordings_ordered, reordered_mics)
            # Compute trilateration to estimate the sound source location
            estimated_position, r1, r2 = localize_source(time_lags, reordered_mics)
            # Log the measurement with a timestamp
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            log_measurement(timestamp, reference_mic, reordered_mics, estimated_position, r1, r2)
            print("Measurement logged.\n----")
    except KeyboardInterrupt:
        print("Terminated by user.")

if __name__ == "__main__":
    main()

