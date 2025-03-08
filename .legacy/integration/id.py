import sounddevice as sd

def list_alsa_mics():
    # Get all host APIs and find those using ALSA
    hostapis = sd.query_hostapis()
    alsa_api_indices = [i for i, api in enumerate(hostapis) if "ALSA" in api['name']]
    
    if not alsa_api_indices:
        logging.info("No ALSA host API found on your system!")
        return

    # Query all devices and logging.info only input devices that belong to ALSA host API(s)
    devices = sd.query_devices()
    logging.info("🎤 Attached Microphones (ALSA IDs):")
    for idx, device in enumerate(devices):
        if device['max_input_channels'] > 0 and device['hostapi'] in alsa_api_indices:
            logging.info(f"  ID {idx}: {device['name']}")

if __name__ == "__main__":
    list_alsa_mics()
    logging.info("remember that killing alsa resets the mic ids")

