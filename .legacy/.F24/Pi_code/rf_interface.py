from pyhackrf import HackRF

# Initialize HackRF devices
hackrf = HackRF()

# Function to receive data
def callback(data, length, context):
    print(f"Received data (length={length}): {data[:10]} ...")  # Print the first 10 bytes
    return 0

try:
    # Iterate through all connected HackRF devices
    for index in range(hackrf.device_count):
        device = hackrf[index]
        print(f"Device {index}: Serial={device.serial_number}")

        # Configure the device (example configuration, modify as needed)
        device.sample_rate = 10e6  # 10 MHz
        device.center_freq = 915e6  # 915 MHz
        device.gain = 40  # Set gain

        # Start receiving
        device.start_rx_mode(callback)

    # Keep the program running
    print("Receiving data from HackRF devices. Press Ctrl+C to stop.")
    while True:
        pass

except KeyboardInterrupt:
    print("Stopping...")
finally:
    hackrf.close()
