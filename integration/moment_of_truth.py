import numpy as np
import sounddevice as sd
from scipy.signal import correlate
import threading
import time
import csv
import os
import socket
from moment2 import ContinuousRecorder


# CONSTANTS
SAMPLE_RATE = 48000          # Hz
WINDOW_DURATION = 1.0        # seconds (sliding window length)
CHUNK_DURATION = 0.1         # seconds (shift amount per iteration)
CHANNELS = 1               # Mono recording
SPEED_OF_SOUND = 343         # m/s

# Global mic order and tower positions.
MIC_ORDER = [4, 2, 3]
# Microphone positions (ALSA card numbers → Physical positions)
MIC_POSITIONS = {
    2: np.array([0, 0]),            # Mic 4
    3: np.array([3.6576, 3.6576]),   # Mic 2
    4: np.array([7.3152, 0])         # Mic 3
}

# CSV logging file
CSV_FILE = "sound_source_log.csv"

# TCP Ports:
#   39440 is used for tower configuration (bi-directional exchange with the tablet)
#   39439 is used to send live (x,y) updates to the tablet.
TOWER_CONFIG_PORT = 39440

# This will be set from the incoming tower configuration connection (i.e. the tablet's IP)
TABLET_IP = None

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

def tower_configuration_server():
    """
    Wait for a connection from the tablet.
    Immediately send the mic indexes (MIC_ORDER) as a comma-separated string.
    Then wait to receive three messages of the form:
        micIndex,x,y
    which will update MIC_POSITIONS.
    """
    global MIC_POSITIONS, TABLET_IP
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('', TOWER_CONFIG_PORT))
    server.listen(1)
    print(f"[Tower Config] Waiting for tower configuration connection on port {TOWER_CONFIG_PORT}...")
    conn, addr = server.accept()
    TABLET_IP = addr[0]
    print(f"[Tower Config] Connected by {TABLET_IP}")
    # Send the mic indexes (e.g., "4,2,3\n")
    mic_indexes_str = ",".join(str(m) for m in MIC_ORDER) + "\n"
    conn.sendall(mic_indexes_str.encode())
    print(f"[Tower Config] Sent mic indexes: {mic_indexes_str.strip()}")

    # Wait to receive tower coordinates until all mics are configured.
    received = {}
    while len(received) < len(MIC_ORDER):
        data = conn.recv(1024)
        if not data:
            break
        lines = data.decode().strip().splitlines()
        for line in lines:
            parts = line.split(',')
            if len(parts) != 3:
                continue
            try:
                mic_index = int(parts[0])
                x = float(parts[1])
                y = float(parts[2])
                received[mic_index] = (x, y)
                print(f"[Tower Config] Received tower coordinate for mic {mic_index}: ({x}, {y})")
            except Exception as e:
                print(f"[Tower Config] Error parsing line '{line}': {e}")
                continue
    # Optionally, send an acknowledgement
    ack = "Tower configuration complete\n"
    conn.sendall(ack.encode())
    conn.close()
    server.close()
    print("[Tower Config] Tower configuration complete. MIC_POSITIONS updated:")
    for mic in MIC_ORDER:
        print(f"   Mic {mic}: {MIC_POSITIONS[mic]}")


def record_mic(mic_id, recordings, duration):
    print(f"[Debug] record_mic: Starting recording on Mic {mic_id} for {duration:.2f} seconds (hw:{mic_id},0)")
    try:
        sd.stop()
        num_samples = int(duration * SAMPLE_RATE)
        print(f"[Debug] record_mic: Expected num_samples = {num_samples}")
        audio_data = sd.rec(num_samples,
                            samplerate=SAMPLE_RATE,
                            channels=CHANNELS,
                            device=f"hw:{mic_id},0",
                            dtype=np.float32)
        print("[Debug] record_mic: Audio recording initiated, waiting for completion.")
        sd.wait()
        print(f"[Debug] record_mic: Raw audio_data shape: {audio_data.shape}")
        
        # Replace NaN values with zero, then clip values to [-1.0, 1.0]
        audio_data = np.nan_to_num(audio_data, nan=0.0)
        print("[Debug] record_mic: After np.nan_to_num - min: {:.6f}, max: {:.6f}".format(np.min(audio_data), np.max(audio_data)))
        
        audio_data = np.clip(audio_data, -1.0, 1.0)
        print("[Debug] record_mic: After clipping - min: {:.6f}, max: {:.6f}".format(np.min(audio_data), np.max(audio_data)))
        
        max_amp = np.max(np.abs(audio_data))
        print(f"[Debug] record_mic: Computed max_amp: {max_amp:.6f}")
        
        if np.isnan(max_amp) or max_amp < 1e-3:
            print(f"[Audio] Warning: Mic {mic_id} recording abnormal (max_amp = {max_amp:.3f}). Using zeros.")
            recordings[mic_id] = np.zeros(num_samples)
        else:
            recordings[mic_id] = audio_data.flatten()
        print(f"[Audio] Mic {mic_id} recorded. Final max amplitude: {np.max(np.abs(recordings[mic_id])):.6f}")
    except Exception as e:
        print(f"[Audio] Error recording Mic {mic_id}: {e}")
        recordings[mic_id] = np.zeros(num_samples)




def record_audio(duration):
    """
    Record 'duration' seconds from all microphones concurrently.
    """
    recordings = {}
    threads = []
    for mic in MIC_ORDER:
        thread = threading.Thread(target=record_mic, args=(mic, recordings, duration))
        thread.start()
        threads.append(thread)
    for thread in threads:
        thread.join()
    return [recordings[mic] for mic in MIC_ORDER]

def determine_reference_mic_cc(recordings_list):
    """
    Determines the reference mic using whole-sample cross-correlation.
    """
    print("[Trilateration] Determining reference mic based on cross-correlation...")
    scores = {}
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
            print(f"[Trilateration] Cross-correlation: Mic {mic} vs Mic {other_mic}: lag = {lag:.6f} s")
        scores[mic] = score
        print(f"[Trilateration] Total score for Mic {mic}: {score:.6f} s")
    reference_mic = max(scores, key=scores.get)
    print(f"[Trilateration] Reference mic chosen: Mic {reference_mic}")
    reordered_mics = [reference_mic] + [m for m in MIC_ORDER if m != reference_mic]
    return reference_mic, reordered_mics

def cross_correlate(recordings_ordered, reordered_mics):
    """
    Computes time delays (lags) between the reference mic and the other mics.
    """
    print("[Trilateration] Computing time lags relative to the reference mic...")
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
        print(f"[Trilateration] Time lag: Ref Mic {reordered_mics[0]} vs Mic {mic}: {time_lag:.6f} s")
    return time_lags

def localize_source(time_lags, reordered_mics):
    """
    Converts time lags to distances and applies trilateration.
    """
    print("[Trilateration] Performing trilateration to locate sound source...")
    if len(reordered_mics) < 3:
        print("[Trilateration] Not enough microphones for trilateration.")
        return np.array([None, None]), 0, 0
    # Convert time lags to distances
    r1 = time_lags.get(reordered_mics[1], 0) * SPEED_OF_SOUND
    r2 = time_lags.get(reordered_mics[2], 0) * SPEED_OF_SOUND
    r3 = 0  # Reference mic
    print(f"[Trilateration] Distances: r1={r1:.3f} m, r2={r2:.3f} m")
    tower1 = MIC_POSITIONS[reordered_mics[0]]
    tower2 = MIC_POSITIONS[reordered_mics[1]]
    tower3 = MIC_POSITIONS[reordered_mics[2]]
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)
    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])
    try:
        estimated_position = np.linalg.solve(A_matrix, b_vector)
        print(f"[Trilateration] Estimated position: x = {estimated_position[0]:.3f}, y = {estimated_position[1]:.3f}")
    except np.linalg.LinAlgError:
        print("[Trilateration] Error: Singular matrix during trilateration.")
        estimated_position = np.array([None, None])
    return estimated_position, r1, r2

def send_location(x, y):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y).
    """
    global TABLET_IP
    if TABLET_IP is None:
        print("[Send] Tablet IP not configured; cannot send location.")
        return
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TABLET_IP, 39439))
        message = f"{x:.2f},{y:.2f}\n"
        sock.sendall(message.encode())
        sock.close()
        print(f"[Send] Sent location: {message.strip()}")
    except Exception as e:
        print(f"[Send] Error sending location: {e}")

def main():
    write_csv_header_if_needed()
    # Step 0: Wait for tower configuration from the tablet.
    tower_configuration_server()

    print("[Main] Initializing continuous recording streams for a sliding window of 1 second...")
    # Create a continuous recorder for each mic
    recorders = {mic: ContinuousRecorder(mic,
                                          samplerate=SAMPLE_RATE,
                                          channels=CHANNELS,
                                          window_duration=WINDOW_DURATION,
                                          chunk_duration=CHUNK_DURATION)
                 for mic in MIC_ORDER}
    
    # Initialize each mic's buffer from its continuous stream.
    mic_buffers = {mic: recorders[mic].buffer for mic in MIC_ORDER}
    for mic in MIC_ORDER:
        print(f"[Init] Mic {mic} initialized. Max amplitude: {np.max(np.abs(mic_buffers[mic])):.3f}")

    print("[Main] Starting live trilateration with sliding window (updating every 0.1 seconds). Press Ctrl+C to stop.")
    try:
        while True:
            start_time = time.time()
            # Update the one-second window from each continuous recorder.
            for mic in MIC_ORDER:
                mic_buffers[mic] = recorders[mic].update_buffer()
            recordings_list = [mic_buffers[mic] for mic in MIC_ORDER]

            # Skip processing if all recordings are essentially silent.
            if all(np.max(np.abs(r)) < 1e-3 for r in recordings_list):
                print("[Main] All microphone recordings are silent. Skipping this iteration.")
                elapsed = time.time() - start_time
                sleep_time = max(0, CHUNK_DURATION - elapsed)
                time.sleep(sleep_time)
                continue

            # Determine reference mic, compute time lags and trilaterate.
            reference_mic, reordered_mics = determine_reference_mic_cc(recordings_list)
            recordings_ordered = [recordings_list[MIC_ORDER.index(mic)] for mic in reordered_mics]
            time_lags = cross_correlate(recordings_ordered, reordered_mics)
            estimated_position, r1, r2 = localize_source(time_lags, reordered_mics)
            if estimated_position[0] is not None:
                send_location(estimated_position[0], estimated_position[1])
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            log_measurement(timestamp, reference_mic, reordered_mics, estimated_position, r1, r2)
            elapsed = time.time() - start_time
            sleep_time = max(0, CHUNK_DURATION - elapsed)
            time.sleep(sleep_time)
    except KeyboardInterrupt:
        print("[Main] Terminated by user.")
    finally:
        for recorder in recorders.values():
            recorder.close()

if __name__ == "__main__":
    main()

