# utils/logger.py
import logging
from config.settings import LOG_FILE

logging.basicConfig(
    level=logging.INFO,
    format="%(asctime)s [%(levelname)s] %(message)s",
    filename=LOG_FILE,  # All logs go to this file
    filemode='a'  # Append to the file; use 'w' to overwrite on each run
)



def log_measurement(timestamp, reference_mic, mic_order, estimated_position, r1, r2):
    logging.info(f"[Log] log_measurement called at {timestamp} with reference_mic: {reference_mic}, mic_order: {mic_order}, estimated_position: {estimated_position}, r1: {r1}, r2: {r2}.")

