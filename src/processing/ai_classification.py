# processing/ai_classification.py

########################################
# REQUIRES PYTHON 3.9 FOR TFLITE TO WORK
########################################

import numpy as np
import librosa
import tflite_runtime.interpreter as tflite

# Load TFLite model and allocate tensors.
interpreter = tflite.Interpreter(model_path='src/processing/model.tflite')
interpreter.allocate_tensors()

# Get input and output tensors.
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()
input_shape = input_details[0]['shape']

#sampling rate of audio
sr = 48000

def classify_audio_sample(audio_sample):
    #convert to nparray
    y=np.array(audio_sample)
    #perform mel spectrogram on audio sample
    mel_spectrogram = librosa.feature.melspectrogram(y=y, sr=sr, n_fft=4096, hop_length=1024, win_length=2048, n_mels=128)
    log_mel_spectrogram = librosa.power_to_db(mel_spectrogram, ref=np.max)
    log_mel_spectrogram = log_mel_spectrogram[:, 2:]

    #reshape melSpec to fit required size, pass data into model
    input_data = log_mel_spectrogram.reshape(input_shape).astype(np.float32)
    interpreter.set_tensor(input_details[0]['index'], input_data)

    interpreter.invoke()

    # The function `get_tensor()` returns a copy of the tensor data.
    output_data = interpreter.get_tensor(output_details[0]['index'])

    #if else branch to determine which type of drone model predicted
    if output_data[0][0] > output_data[0][1] and output_data[0][0] > output_data[0][2]:
        return "VistaTech"
    elif output_data[0][1] > output_data[0][0] and output_data[0][1] > output_data[0][2]:
        return "Holy Stone"
    else:
        return "No Drone"