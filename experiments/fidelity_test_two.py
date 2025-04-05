# I FIGURED THE BEST WAY TO PERFORM AN EXPERIMENT THAT REPLICATE PUR PROJECT WAS TO MODIFY OUR MAIN WORKFLOW

########################## THIS FILE IS A HEAVILY MODIED VERSION OF main.py from 
#bd071ab96e6a603c04fb2b2e9d4cbfb9b8bec293
import time
import logging

# Import configuration parameters
from config.settings import SAMPLE_RATE, WINDOW_DURATION, CHUNK_DURATION, MIC_ORDER, MIC_POSITIONS

# Import submodules for functionality using relative imports
from src.sensors.audio_recorder import ContinuousRecorder
from src.processing.trilateration import analyze_microphones, localize_source
from src.communications.tower_config import tower_configuration_server
from src.communications.tablet_comm import send_location, send_classification
from src.utils.logger import log_measurement
from src.processing.ai_classification import classify_audio_sample  # For AI audio analysis

# Placeholders for future integration:
#from .communications.encryption import encrypt_message, decrypt_message  # For secure messaging
#from .sensors.rf_receiver import start_rf_listener  # For handling RF data reception


def main():
    # Set up logging for debugging and info messages
    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

    # Step 1: Configure tower settings via a TCP connection with the tablet.
    # This call blocks until the tower configuration (handshake + coordinate messages) is complete.
    # tower_configuration_server()

    # Step 6: After configuration, enter the main loop for continuous audio processing.
    # (For now, the main loop is provided as a placeholder; adapt as needed.)
    logging.info("Starting main loop for continuous audio processing...")
    recorders = {
        mic: ContinuousRecorder(mic, samplerate=SAMPLE_RATE, channels=1,
                                window_duration=WINDOW_DURATION,
                                chunk_duration=CHUNK_DURATION)
        for mic in MIC_ORDER
    }
    mic_buffers = {mic: recorders[mic].buffer for mic in MIC_ORDER}

    try:
        while True:
            # This is a checker that will not let any processing happen if the positions have not been set or sent to the configs
            if any(pos[0] is None or pos[1] is None for pos in MIC_POSITIONS.values()):
                    logging.info("[INFO] - Mic positions not yet fully defined. Waiting...")
                    time.sleep(CHUNK_DURATION)
                    continue
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
            reference_mic, reordered_mics, time_lags = analyze_microphones(recordings_list)
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

