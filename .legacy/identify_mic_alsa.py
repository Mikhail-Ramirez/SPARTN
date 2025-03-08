import subprocess
import time

def get_alsa_cards():
    """Get a list of available ALSA capture devices (microphones)."""
    result = subprocess.run(["arecord", "-l"], capture_output=True, text=True)
    lines = result.stdout.split("\n")
    
    devices = []
    for line in lines:
        if "card" in line and "USB Audio" in line:
            parts = line.split(":")
            card_number = parts[0].split()[1]  # Extract the card number
            device_name = parts[1].strip().split("[")[1].split("]")[0]  # Extract the device name
            devices.append((card_number, device_name))
    
    return devices

def detect_new_mics():
    """Continuously check for newly plugged-in microphones."""
    print("Please start with NO microphones plugged in, then add them one by one.")
    print("Waiting for microphones...")

    previous_mics = set()
    
    while True:
        current_mics = set(get_alsa_cards())

        # Check for new microphones
        new_mics = current_mics - previous_mics
        if new_mics:
            for mic in new_mics:
                print(f"New Microphone Detected! Card: {mic[0]}, Name: {mic[1]}")

        previous_mics = current_mics

        time.sleep(3)  # Check every 3 seconds

try:
    detect_new_mics()
except KeyboardInterrupt:
    print("\nScript stopped.")

