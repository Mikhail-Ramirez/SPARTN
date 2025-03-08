import numpy as np
import sounddevice as sd
import time

SAMPLE_RATE = 48000          # Hz
WINDOW_DURATION = 1.0        # seconds for the sliding window
CHUNK_DURATION = 0.1         # seconds per update

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

# Example usage in your main loop:
def process_location(audio_buffer):
    # Dummy processing function: replace with your existing location logic.
    max_amp = np.max(np.abs(audio_buffer))
    print(f"[Location] Processed 1-sec buffer, max amplitude: {max_amp:.6f}")
    # Return estimated (x, y) location (dummy values here)
    return (0.0, 0.0)

def main():
    # Suppose MIC_ORDER is [4, 2, 3] as in your script.
    mic_ids = [4, 2, 3]
    # Create a recorder for each microphone.
    recorders = {mic: ContinuousRecorder(mic) for mic in mic_ids}

    try:
        while True:
            buffers = {}
            for mic in mic_ids:
                # Update the one-second window from each microphone.
                buffers[mic] = recorders[mic].update_buffer()
            # For instance, use one mic’s buffer (or combine channels as needed)
            location = process_location(buffers[mic_ids[0]])
            # Here you could call your send_location(location) function.
            # e.g., send_location(location)
            time.sleep(CHUNK_DURATION)
    except KeyboardInterrupt:
        print("[Main] Terminated by user.")
    finally:
        for recorder in recorders.values():
            recorder.close()

if __name__ == "__main__":
    main()

