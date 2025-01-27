import socket
import math
import time

# Configuration
SERVER_IP = '10.0.2.16'  # IP address of reciver
SERVER_PORT = 39439         # Port should match the app's listening port
RADIUS = 25                # Radius of the circle
FREQUENCY = 30             # Updates per second (30 Hz)

def main():
    print("Starting packet sender...")
    # Create a TCP/IP socket
    sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    sock.connect((SERVER_IP, SERVER_PORT))
    print("Connected to the Kivy app...")

    # Start timing loop
    angle = 0  # Start at angle 0
    update_interval = 1.0 / FREQUENCY  # Time between updates
    print("Sending data...")

    try:
        while True:
            # Calculate x and y based on the angle (in radians)
            x = RADIUS * math.cos(angle)
            y = RADIUS * math.sin(angle)

            # Send the x, y coordinates as a comma-separated string
            message = f"{x:.2f},{y:.2f}\n"
            sock.sendall(message.encode())
            print(f"Sent: {message.strip()}")

            # Update the angle to create circular motion
            angle += 2 * math.pi * update_interval  # Rotate at a fixed rate for 30 Hz

            # Wait for the next frame
            time.sleep(update_interval)

    except KeyboardInterrupt:
        print("Stopping script.")

    finally:
        sock.close()

if __name__ == '__main__':
    main()

