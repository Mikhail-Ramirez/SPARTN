import numpy as np
import sounddevice as sd
import matplotlib.pyplot as plt
from scipy.io.wavfile import write
from scipy.signal import correlate
import threading

# **🔹 CONSTANTS**
SAMPLE_RATE = 48000  # Hz
DURATION = 10  # **Fixed at 2 seconds**
CHANNELS = 1  # **Mono Recording**
SPEED_OF_SOUND = 343  # m/s

# **Microphone positions (ALSA card numbers → Physical positions)**
MIC_POSITIONS = {
    2: np.array([0,0]),  # **Mic 4**
    3: np.array([3.6576,3.6576]),  # **Mic 2**
    4: np.array([7.3152, 0])   # **Mic 3**
}

# **Recording Order**
MIC_ORDER = [4, 2, 3]  # Default order, will change dynamically

# **Storage for recorded audio**
recordings = {}

def record_mic(mic_id):
    """Records audio from a single microphone in a separate thread."""
    global recordings
    print(f"🔹 Starting recording on Mic {mic_id} (hw:{mic_id},0)")

    try:
        audio_data = sd.rec(
            int(DURATION * SAMPLE_RATE),
            samplerate=SAMPLE_RATE,
            channels=1,
            device=f"hw:{mic_id},0",
            dtype=np.float32
        )
        sd.wait()

        recordings[mic_id] = audio_data.flatten()
        write(f"mic_{mic_id}.wav", SAMPLE_RATE, recordings[mic_id])
        print(f"✅ Mic {mic_id} recorded. Max Amplitude: {np.max(np.abs(audio_data)):.3f}")

    except Exception as e:
        print(f"❌ Error recording Mic {mic_id}: {e}")
        recordings[mic_id] = np.zeros(int(DURATION * SAMPLE_RATE))  # Store empty data to avoid crashes


def record_audio():
    """Starts recording for all microphones simultaneously."""
    print(f"🎙️ Recording from all microphones **SIMULTANEOUSLY** for {DURATION} seconds...")

    threads = []
    for mic in MIC_ORDER:
        thread = threading.Thread(target=record_mic, args=(mic,))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()  # **Wait for all recordings to finish**

    return [recordings[mic] for mic in MIC_ORDER]

def determine_reference_mic(recordings):
    """Dynamically assigns the reference microphone based on the **earliest significant signal**."""
    print("\n🎯 Determining Reference Mic...")

    first_signal_indices = {}
    plt.figure(figsize=(10, 6))

    for i, mic in enumerate(MIC_ORDER):
        signal = np.abs(recordings[i])
        threshold = np.max(signal) * 0.1  # **10% of peak amplitude as threshold**
        signal_above_threshold = np.where(signal > threshold)[0]

        if len(signal_above_threshold) > 0:
            first_signal_indices[mic] = signal_above_threshold[0]
        else:
            first_signal_indices[mic] = len(signal)  # No signal detected, place at end

        # **Plot detected signal**
        plt.plot(signal, label=f"Mic {mic} (Start: {first_signal_indices[mic]} samples)")

    plt.xlabel("Samples")
    plt.ylabel("Amplitude")
    plt.title("Microphone Signal Detection")
    plt.legend()
    plt.savefig("mic_signal_detection.png")
    print("✅ Saved: mic_signal_detection.png")
    plt.show()

    # **Choose the mic that detected sound first**
    reference_mic = min(first_signal_indices, key=first_signal_indices.get)

    print(f"\n🎯 **New Reference Mic Assigned: Mic {reference_mic}** (Detected First Signal at {first_signal_indices[reference_mic]} samples)")
    for mic, sample_index in first_signal_indices.items():
        print(f"   - Mic {mic}: First detected signal at sample {sample_index}")

    reordered_mics = [reference_mic] + [mic for mic in MIC_ORDER if mic != reference_mic]
    return reference_mic, reordered_mics


def plot_audio(recordings, reordered_mics):
    """Plots waveforms of the recorded audio."""
    print("\n📊 Plotting recorded audio signals...")
    time_axis = np.linspace(0, DURATION, len(recordings[0]))

    plt.figure(figsize=(10, 6))
    for i, mic in enumerate(reordered_mics):
        plt.subplot(3, 1, i + 1)
        plt.plot(time_axis, recordings[i], label=f"Mic {mic} @ {MIC_POSITIONS[mic]}")
        plt.ylabel("Amplitude")
        plt.legend()

    plt.xlabel("Time (s)")
    plt.suptitle("Synchronized Recorded Audio Waveforms")
    plt.tight_layout()
    plt.savefig("audio_waveforms_synced.png")
    print("✅ Saved: audio_waveforms_synced.png")
    plt.show()

def cross_correlate(recordings, reordered_mics):
    """Computes time delays between microphones using cross-correlation."""
    print("\n🔍 Performing cross-correlation to find time lags...")

    ref_mic = reordered_mics[0]  # **Reference mic**
    ref_signal = recordings[0]
    time_lags = {}

    plt.figure(figsize=(10, 4))

    for i in range(1, len(recordings)):
        mic = reordered_mics[i]
        mic_signal = recordings[i]

        correlation = correlate(ref_signal, mic_signal, mode='full')
        lags = np.arange(-len(ref_signal) + 1, len(ref_signal)) / SAMPLE_RATE  # Convert samples to time

        # **Find the peak of cross-correlation**
        peak_lag_index = np.argmax(np.abs(correlation))
        time_lag = lags[peak_lag_index]

        # **Keep time lags consistent with actual arrival order**
        time_lags[mic] = time_lag  

        print(f"🔹 Time lag between Mic {ref_mic} (Ref) and Mic {mic}: {time_lag:.6f} seconds")
        print(f"   - Peak Correlation Index: {peak_lag_index}, Lag (s): {time_lag:.6f}")

        # **Plot cross-correlation**
        plt.plot(lags, correlation, label=f"Mic {mic} vs Ref (Mic {ref_mic})")
        plt.axvline(x=time_lag, color='r', linestyle='--', label=f"Detected Lag: {time_lag:.6f}s")

    plt.xlabel("Time Lag (s)")
    plt.ylabel("Cross-Correlation")
    plt.title("Cross-Correlation of Microphone Signals")
    plt.legend()
    plt.grid()
    plt.savefig("cross_correlation_plot.png")
    print("✅ Saved: cross_correlation_plot.png")
    plt.show()

    return time_lags, ref_mic

def localize_source(time_lags, ref_mic):
    """Uses trilateration to estimate the sound source location."""
    print("\n📍 Performing trilateration to locate sound source...")

    # **Convert time lags to distances**
    r1 = time_lags.get(MIC_ORDER[1], 0) * SPEED_OF_SOUND  # Mic 2 distance
    r2 = time_lags.get(MIC_ORDER[2], 0) * SPEED_OF_SOUND  # Mic 3 distance
    r3 = 0  # **Reference mic has no lag**

    print(f"   - Distances (meters): r1={r1:.3f}, r2={r2:.3f}")

    # **Known positions**
    tower1 = MIC_POSITIONS[ref_mic]
    tower2 = MIC_POSITIONS[MIC_ORDER[1]]
    tower3 = MIC_POSITIONS[MIC_ORDER[2]]

    # **Trilateration calculation**
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)

    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])

    try:
        estimated_position = np.linalg.solve(A_matrix, b_vector)
        print(f"✅ Estimated position of the sound source: {estimated_position}")
        plot_localization(tower1, tower2, tower3, estimated_position)
    except np.linalg.LinAlgError:
        print("❌ ERROR: Trilateration failed due to singular matrix.")
        estimated_position = np.array([None, None])

    return estimated_position


def plot_time_lag_debug(time_lags):
    """Plot the computed time lags to verify correctness."""
    plt.figure(figsize=(6, 4))

    mics = list(time_lags.keys())
    lags = list(time_lags.values())

    plt.bar(mics, lags, color=['blue', 'green'])
    plt.xlabel("Microphones")
    plt.ylabel("Time Lag (s)")
    plt.title("Computed Time Lags")
    plt.grid(True)
    plt.savefig("time_lag_debug.png")
    print("✅ Saved: time_lag_debug.png")
    plt.show()




def plot_localization(tower1, tower2, tower3, estimated_position):
    """Plots the microphone positions and estimated sound source."""
    plt.figure(figsize=(8, 8))

    plt.scatter(*tower1, color='r', label="Mic 4 (Ref)", s=100)
    plt.scatter(*tower2, color='g', label="Mic 2", s=100)
    plt.scatter(*tower3, color='b', label="Mic 3", s=100)
    plt.scatter(*estimated_position, color='m', marker='x', s=150, label="Estimated Sound Source")

    plt.annotate("Mic 4 (Ref)", tower1, textcoords="offset points", xytext=(-10,-10), ha='center')
    plt.annotate("Mic 2", tower2, textcoords="offset points", xytext=(-10,-10), ha='center')
    plt.annotate("Mic 3", tower3, textcoords="offset points", xytext=(-10,-10), ha='center')

    plt.xlim(-1, 2)
    plt.ylim(-1, 2)
    plt.xlabel("X Position (meters)")
    plt.ylabel("Y Position (meters)")
    plt.title("Sound Source Localization")
    plt.grid(True)
    plt.legend()
    plt.savefig("trial_sound_source_localization_debug.png")
    print("✅ Saved: trial_sound_source_localization_debug.png")
    plt.show()




def main():
    recordings_list = record_audio()
    ref_mic, reordered_mics = determine_reference_mic(recordings_list)
    plot_audio(recordings_list, reordered_mics)
    time_lags, ref_mic = cross_correlate(recordings_list, reordered_mics)
    localize_source(time_lags, ref_mic)


if __name__ == "__main__":
    main()

