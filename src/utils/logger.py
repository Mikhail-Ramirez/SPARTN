# utils/logger.py
import logging
import sys
from config.settings import LOG_FILE

# Create a root logger
logger = logging.getLogger()
logger.setLevel(logging.INFO)

# File handler: writes logs to a file
file_handler = logging.FileHandler(LOG_FILE, mode='a')
file_handler.setFormatter(logging.Formatter("%(asctime)s %(message)s"))
logger.addHandler(file_handler)

# Stream handler: prints logs to the terminal (stdout)
stream_handler = logging.StreamHandler(sys.stdout)
stream_handler.setFormatter(logging.Formatter("%(asctime)s %(message)s"))
logger.addHandler(stream_handler)


def log_measurement(timestamp, reference_mic, mic_order, estimated_position, r1, r2):
    logging.info(f"[Log] log_measurement called at {timestamp} with reference_mic: {reference_mic}, mic_order: {mic_order}, estimated_position: {estimated_position}, r1: {r1}, r2: {r2}.")

