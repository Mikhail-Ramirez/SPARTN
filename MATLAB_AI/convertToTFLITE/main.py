import os

os.environ['TF_ENABLE_ONEDNN_OPTS'] = '0'

import multClassModel_latest
import tensorflow as tf
import numpy as np
import scipy.io

model = multClassModel_latest.load_model()

#check that tensorflow model is correct
mat_file = scipy.io.loadmat('test.mat')
loaded_arr = mat_file['X']
print(loaded_arr.shape)
loaded_arr = loaded_arr.reshape(-1, 128, 45, 1)
print(loaded_arr.shape)
prediction = model.predict(loaded_arr)
print('\n')
print(prediction)

#model.export('savedModel')

# Convert the model
converter = tf.lite.TFLiteConverter.from_keras_model(model) # path to the SavedModel directory
tflite_model = converter.convert()

# Save the model.
with open('model.tflite', 'wb') as f:
  f.write(tflite_model)

interpreter = tf.lite.Interpreter(model_path="model.tflite")
interpreter.allocate_tensors()
input_details = interpreter.get_input_details()
output_details = interpreter.get_output_details()

#input_shape = input_details[0]['shape']
#input_data = np.random.random_sample(input_shape).astype(np.float32)

interpreter.set_tensor(input_details[0]['index'], loaded_arr.astype(np.float32))
interpreter.invoke()
output_data = interpreter.get_tensor(output_details[0]['index'])
print(output_data)