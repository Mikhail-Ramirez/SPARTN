# src/main.py
import time
import logging

#Threading in an attempt to speed up processing values
from concurrent.futures import ThreadPoolExecutor, ProcessPoolExecutor, as_completed

# Import configuration parameters
from config.settings import SAMPLE_RATE, WINDOW_DURATION, CHUNK_DURATION, MIC_ORDER, MIC_POSITIONS

# Import submodules for functionality using relative imports
from .sensors.audio_recorder import ContinuousRecorder
from .processing.trilateration import analyze_microphones, localize_source, get_loudest, get_shift_percentages
from .communications.tower_config import tower_configuration_server
from .communications.tablet_comm import send_location, send_classification, send_quadrant, send_shift_values
from .utils.logger import log_measurement
from .processing.ai_classification import classify_audio_sample  # For AI audio analysis

# Placeholders for future integration:
#from .communications.encryption import encrypt_message, decrypt_message  # For secure messaging
#from .sensors.rf_receiver import start_rf_listener  # For handling RF data reception


def main():
    # Set up logging for debugging and info messages
    logging.basicConfig(level=logging.INFO, format="%(asctime)s [%(levelname)s] %(message)s")

    # Step 1: Configure tower settings via a TCP connection with the tablet.
    # This call blocks until the tower configuration (handshake + coordinate messages) is complete.
    tower_configuration_server()

    # Step 6: After configuration, enter the main loop for continuous audio processing.
    # (For now, the main loop is provided as a placeholder; adapt as needed.)
    logging.info("Starting main loop for continuous audio processing...")
    recorders = {
        mic: ContinuousRecorder(mic, samplerate=SAMPLE_RATE, channels=1,
                                window_duration=WINDOW_DURATION,
                                chunk_duration=CHUNK_DURATION)
        for mic in MIC_ORDER
    }

    #mic_buffers = {mic: recorders[mic].buffer for mic in MIC_ORDER}
    #time_diffs = []

    # 3) Executors
    record_executor = ThreadPoolExecutor(max_workers=len(MIC_ORDER))
    # We only need two workers: one for shift‐computation, one for loudest
    proc_executor   = ProcessPoolExecutor(max_workers=3)
    try:
        while True:
            prev_time = time.time()

            # SAMPLING MICROPHONES+++++++++++++++++++++++++++++++++++++++++++++++
            # This is a checker that will not let any processing happen if the positions have not been set or sent to the configs
            if any(pos[0] is None or pos[1] is None for pos in MIC_POSITIONS.values()):
                    logging.info("[INFO] - Mic positions not yet fully defined. Waiting...")
                    time.sleep(CHUNK_DURATION)
                    continue

            # --- Parallel record/update ---
            record_futures = {
                record_executor.submit(rec.update_buffer): mic
                for mic, rec in recorders.items()
            }
            mic_buffers = {}
            for future in as_completed(record_futures):
                mic = record_futures[future]
                try:
                    mic_buffers[mic] = future.result()
                except Exception as e:
                    logging.error(f"Mic {mic} update failed: {e}")

            recordings = [mic_buffers[m] for m in MIC_ORDER]

            # Skip if completely silent
            if all(max(abs(r)) < 1e-3 for r in recordings):
                logging.debug("Silence; skipping iteration.")
                time.sleep(CHUNK_DURATION)
                continue

            # --- Parallel processing (CPU‐bound) ---
            shift_fut   = proc_executor.submit(get_shift_percentages, recordings)
            loudest_fut = proc_executor.submit(get_loudest, recordings)

            shift_values       = shift_fut.result()
            estimated_quadrant = loudest_fut.result()

            # --- Send & log (I/O) ---
            send_shift_values(shift_values)
            send_quadrant(estimated_quadrant)

            # AI CLASSIFICAITON ++++++++++++++++++++++++++++++++++++++++++++++++++++++
            classification_result = "No Classification"  #default for when no class

            class_fut   = proc_executor.submit(classify_audio_sample, recordings_list[0])
            classification_result = class_fut.result()

            logging.info(f"AI Classification Result: {classification_result}")
            send_classification(classification_result)
            # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


            # LOGGING AND TIME ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            # Log the measurement to file with a timestamp
            timestamp = time.strftime("%Y-%m-%d %H:%M:%S")
            # dummy placeholders for your old variables:
            reference_mic, reordered_mics, r1, r2 = None, None, 0, 0
            log_measurement(
                timestamp, reference_mic,
                reordered_mics, estimated_quadrant,
                r1, r2
            )

            #print(f"{time.time() - prev_time}")
            #time_diffs.append(time.time() - prev_time)
            #print(sum(time_diffs) / len(time_diffs))
            # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


            # RF PROCESSING ++++++++++++++++++++++++++++++++++++++++++++++++++++++++
            # Optional: Start or process RF data if needed (this could be running on a separate thread)
            # start_rf_listener()
            # ++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++


    except KeyboardInterrupt:
        logging.info("Main loop terminated by user.")
    finally:
        # Ensure that all continuous recorders are properly closed
        for recorder in recorders.values():
            recorder.close()

if __name__ == "__main__":
    main()

