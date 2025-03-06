# src/main.py

import time
import logging

# Import configuration parameters
from config.settings import SAMPLE_RATE, WINDOW_DURATION, CHUNK_DURATION, MIC_ORDER

# Import submodules for functionality
from sensors.audio_recorder import ContinuousRecorder
from processing.trilateration import determine_reference_mic, cross_correlate, localize_source
from communications.tower_config import tower_configuration_server
from communications.tablet_comm import send_location
from utils.logger import write_csv_header_if_needed, log_measurement
# Placeholders for future integration:
from processing.ai_classification import classify_audio_sample  # For AI audio analysis
from communications.encryption import encrypt_message, decrypt_message  # For secure messaging
from sensors.rf_receiver import start_rf_listener  # For handling RF data reception

def main():
    # Set up logging for debugging and info messages
    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")
    
    # Write CSV header if necessary
    write_csv_header_if_needed()
    
    # Step 0: Configure tower settings via a TCP connection with the tablet
    tower_configuration_server()
    
    # Initialize continuous recorders for each microphone
    recorders = {
        mic: ContinuousRecorder(mic, samplerate=SAMPLE_RATE, channels=1,
                                window_duration=WINDOW_DURATION,
                                chunk_duration=CHUNK_DURATION)
        for mic in MIC_ORDER
    }
    mic_buffers = {mic: recorders[mic].buffer for mic in MIC_ORDER}
    
    logging.info("Starting main loop for continuous audio processing...")
    
    try:
        while True:
            loop_start = time.time()
            
            # Update buffers from each recorder
            for mic in MIC_ORDER:
                mic_buffers[mic] = recorders[mic].update_buffer()
            recordings_list = [mic_buffers[mic] for mic in MIC_ORDER]
            
            # Skip processing if all microphones are silent
            if all(max(abs(r)) < 1e-3 for r in recordings_list):
                logging.debug("All microphones silent. Skipping iteration.")
                time.sleep(CHUNK_DURATION)
                continue
            
            # Determine the reference microphone using cross-correlation
            reference_mic, reordered_mics = determine_reference_mic(recordings_list)
            # Reorder recordings to match the mic order determined above
            recordings_ordered = [recordings_list[MIC_ORDER.index(mic)] for mic in reordered_mics]
            
            # Calculate time lags between the reference and other microphones
            time_lags = cross_correlate(recordings_ordered, reordered_mics)
            # Estimate the source location via trilateration
            estimated_position, r1, r2 = localize_source(time_lags, reordered_mics)
            
            if estimated_position[0] is not None:
                # Encrypt the message if needed before sending
                location_message = f"{estimated_position[0]:.2f},{estimated_position[1]:.2f}\n"
                encrypted_message = encrypt_message(location_message)
                send_location(encrypted_message)
            
            # Log the measurement to file with a timestamp
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S", time.localtime())
            log_measurement(timestamp, reference_mic, reordered_mics, estimated_position, r1, r2)
            
            # Optional: Extract a 1-second audio sample and classify it using the AI system.
            # classification_result = classify_audio_sample(recordings_ordered[0])
            # logging.info(f"AI Classification Result: {classification_result}")
            
            # Optional: Start or process RF data if needed (this could be running on a separate thread)
            # start_rf_listener()
            
            # Maintain loop rate based on CHUNK_DURATION
            elapsed = time.time() - loop_start
            sleep_time = max(0, CHUNK_DURATION - elapsed)
            time.sleep(sleep_time)
    
    except KeyboardInterrupt:
        logging.info("Main loop terminated by user.")
    
    finally:
        # Ensure that all continuous recorders are properly closed
        for recorder in recorders.values():
            recorder.close()

if __name__ == "__main__":
    main()

