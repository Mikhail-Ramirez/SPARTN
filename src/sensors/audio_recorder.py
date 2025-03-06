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
