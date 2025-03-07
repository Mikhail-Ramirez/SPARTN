# communications/tower_config.py
import socket

from config.settings import *

def tower_configuration_server():
    """
    Wait for a connection from the tablet.
    Immediately send the mic indexes (MIC_ORDER) as a comma-separated string.
    Then wait to receive three messages of the form:
        micIndex,x,y
    which will update MIC_POSITIONS.
    """
    global MIC_POSITIONS, TABLET_IP
    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('', TOWER_CONFIG_PORT))
    server.listen(1)
    print(f"[Tower Config] Waiting for tower configuration connection on port {TOWER_CONFIG_PORT}...")
    conn, addr = server.accept()
    TABLET_IP = addr[0]
    print(f"[Tower Config] Connected by {TABLET_IP}")
    # Send the mic indexes (e.g., "4,2,3\n")
    mic_indexes_str = ",".join(str(m) for m in MIC_ORDER) + "\n"
    conn.sendall(mic_indexes_str.encode())
    print(f"[Tower Config] Sent mic indexes: {mic_indexes_str.strip()}")

    # Wait to receive tower coordinates until all mics are configured.
    received = {}
    while len(received) < len(MIC_ORDER):
        data = conn.recv(1024)
        if not data:
            break
        lines = data.decode().strip().splitlines()
        for line in lines:
            parts = line.split(',')
            if len(parts) != 3:
                continue
            try:
                mic_index = int(parts[0])
                x = float(parts[1])
                y = float(parts[2])
                received[mic_index] = (x, y)
                print(f"[Tower Config] Received tower coordinate for mic {mic_index}: ({x}, {y})")
            except Exception as e:
                print(f"[Tower Config] Error parsing line '{line}': {e}")
                continue
    # Optionally, send an acknowledgement
    ack = "Tower configuration complete\n"
    conn.sendall(ack.encode())
    conn.close()
    server.close()
    print("[Tower Config] Tower configuration complete. MIC_POSITIONS updated:")
    for mic in MIC_ORDER:
        print(f"   Mic {mic}: {MIC_POSITIONS[mic]}")
