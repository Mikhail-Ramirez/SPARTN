# sensors/audio_recorder.py
from config.settings import SAMPLE_RATE, WINDOW_DURATION, CHUNK_DURATION, MIC_ORDER
import numpy as np
import sounddevice as sd

class ContinuousRecorder:
    def __init__(self, mic_id, samplerate=SAMPLE_RATE, channels=1,
                 window_duration=WINDOW_DURATION, chunk_duration=CHUNK_DURATION):
        self.mic_id = mic_id
        self.samplerate = samplerate
        self.channels = channels
        self.window_duration = window_duration
        self.chunk_duration = chunk_duration
        self.buffer_size = int(window_duration * samplerate)
        self.chunk_size = int(chunk_duration * samplerate)
        # initialize the one-second buffer with zeros
        self.buffer = np.zeros(self.buffer_size, dtype=np.float32)
        # open the stream once using the hardware device string
        self.stream = sd.InputStream(samplerate=samplerate,
                                     channels=channels,
                                     device=f"hw:{mic_id},0",
                                     dtype='float32')
        self.stream.start()
        print(f"[Recorder] Mic {mic_id} continuous stream started.")

    def update_buffer(self):
        # read a small chunk from the stream
        chunk, _ = self.stream.read(self.chunk_size)
        chunk = chunk.flatten()
        # slide the window: drop the oldest samples, append the new chunk
        self.buffer = np.concatenate((self.buffer[self.chunk_size:], chunk))
        return self.buffer

    def close(self):
        self.stream.stop()
        self.stream.close()
        print(f"[Recorder] Mic {self.mic_id} stream closed.")




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
