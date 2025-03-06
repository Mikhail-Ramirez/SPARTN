# utils/logger.py

def write_csv_header_if_needed():
    print("[Dummy] write_csv_header_if_needed called.")

def log_measurement(timestamp, reference_mic, mic_order, estimated_position, r1, r2):
    print(f"[Dummy] log_measurement called at {timestamp} with reference_mic: {reference_mic}, mic_order: {mic_order}, estimated_position: {estimated_position}, r1: {r1}, r2: {r2}.")

