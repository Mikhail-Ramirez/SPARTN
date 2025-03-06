import numpy as np
import sounddevice as sd
import matplotlib.pyplot as plt
from scipy.io.wavfile import write
from scipy.signal import correlate
import threading

# Constants
SAMPLE_RATE = 44100  # Hz
DURATION = 10  # seconds

# Microphone mappings (ALSA card numbers → Physical locations)
MIC_POSITIONS = {
    2: np.array([0, 0]),  
    3: np.array([3.657, 3.657]), 
    4: np.array([7.3152, 0]) 
}

# Order of microphones (Matches ALSA card numbers)
MIC_ORDER = [2, 4, 3]

# Storage for recorded audio
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
    print("Recording from all microphones **SIMULTANEOUSLY** for 10 seconds...")

    threads = []
    for mic in MIC_ORDER:
        thread = threading.Thread(target=record_mic, args=(mic,))
        thread.start()
        threads.append(thread)

    for thread in threads:
        thread.join()  # Wait for all recordings to finish

    return [recordings[mic] for mic in MIC_ORDER]


def plot_audio(recordings):
    """Plots waveforms of the recorded audio."""
    print("Plotting recorded audio signals...")
    time_axis = np.linspace(0, DURATION, len(recordings[0]))

    plt.figure(figsize=(10, 6))
    for i, mic in enumerate(MIC_ORDER):
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


def cross_correlate(recordings):
    """Computes time delays between microphones using cross-correlation."""
    print("Performing cross-correlation to find time lags...")

    mic1 = recordings[0]  # Reference mic
    time_lags = {}

    for i in range(1, len(recordings)):
        mic_other = recordings[i]
        correlation = correlate(mic1, mic_other, mode='full')
        lag_samples = np.argmax(correlation) - (len(mic1) - 1)
        time_lag = lag_samples / SAMPLE_RATE
        time_lags[MIC_ORDER[i]] = time_lag
        print(f"🔹 Time lag between Mic {MIC_ORDER[0]} and Mic {MIC_ORDER[i]}: {time_lag:.6f} seconds")

    return time_lags


def localize_source(time_lags):
    """Uses trilateration to estimate the sound source location."""
    print("Performing trilateration to locate sound source...")
    speed_of_sound = 343  # m/s

    # Convert time lags to distances
    r1 = abs(time_lags.get(4, 0)) * speed_of_sound
    r2 = abs(time_lags.get(3, 0)) * speed_of_sound
    r3 = 0  # Mic 1 is the reference (no lag)

    # Known positions
    tower1 = MIC_POSITIONS[2]
    tower2 = MIC_POSITIONS[4]
    tower3 = MIC_POSITIONS[3]

    # Trilateration calculation
    A = 2 * (tower2 - tower1)
    B = 2 * (tower3 - tower1)
    C = r1**2 - r2**2 - np.dot(tower1, tower1) + np.dot(tower2, tower2)
    D = r1**2 - r3**2 - np.dot(tower1, tower1) + np.dot(tower3, tower3)

    A_matrix = np.array([A, B]).T
    b_vector = np.array([C, D])

    estimated_position = np.linalg.solve(A_matrix, b_vector)

    print(f"✅ Estimated position of the sound source: {estimated_position}")

    # Plot results
    plt.figure(figsize=(8, 8))
    plt.scatter(*tower1, color='r', label="Mic 1 (Ref)")
    plt.scatter(*tower2, color='g', label="Mic 2")
    plt.scatter(*tower3, color='b', label="Mic 3")
    plt.scatter(*estimated_position, color='m', marker='x', label="Estimated Source")

    plt.xlim(-1, 2)
    plt.ylim(-1, 2)
    plt.grid(True)
    plt.legend()
    plt.title("Sound Source Localization")
    plt.xlabel("X Position (meters)")
    plt.ylabel("Y Position (meters)")
    plt.savefig("sound_source_localization_synced.png")
    print("✅ Saved: sound_source_localization_synced.png")
    plt.show()


def main():
    recordings_list = record_audio()
    plot_audio(recordings_list)
    time_lags = cross_correlate(recordings_list)
    localize_source(time_lags)


if __name__ == "__main__":
    main()

