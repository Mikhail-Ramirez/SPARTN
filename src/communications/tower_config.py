# communications/tower_config.py
import socket
import logging
from config.settings import *
from communications.encryption import rsa_decrypt, decrypt_message, exchange_keys_with_tablet

# def tower_configuration_server():
#     """
#     Wait for a connection from the tablet.
#     Immediately send the mic indexes (MIC_ORDER) as a comma-separated string.
#     Then wait to receive three messages of the form:
#         micIndex,x,y
#     which will update MIC_POSITIONS.
#     """
#     global MIC_POSITIONS, TABLET_IP
#     server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#     server.bind(('', TOWER_CONFIG_PORT))
#     server.listen(1)
#     logging.info(f"[Tower Config] Waiting for tower configuration connection on port {TOWER_CONFIG_PORT}...")
#     conn, addr = server.accept()
#     TABLET_IP = addr[0]
#     logging.info(f"[Tower Config] Connected by {TABLET_IP}")
#     # Send the mic indexes (e.g., "4,2,3\n")
#     mic_indexes_str = ",".join(str(m) for m in MIC_ORDER) + "\n"
#     conn.sendall(mic_indexes_str.encode())
#     logging.info(f"[Tower Config] Sent mic indexes: {mic_indexes_str.strip()}")

#     # Wait to receive tower coordinates until all mics are configured.
#     received = {}
#     while len(received) < len(MIC_ORDER):
#         data = conn.recv(1024)
#         if not data:
#             break
#         lines = data.decode().strip().splitlines()
#         for line in lines:
#             parts = line.split(',')
#             if len(parts) != 3:
#                 continue
#             try:
#                 mic_index = int(parts[0])
#                 x = float(parts[1])
#                 y = float(parts[2])
#                 received[mic_index] = (x, y)
#                 logging.info(f"[Tower Config] Received tower coordinate for mic {mic_index}: ({x}, {y})")
#             except Exception as e:
#                 logging.info(f"[Tower Config] Error parsing line '{line}': {e}")
#                 continue
#     # Optionally, send an acknowledgement
#     ack = "Tower configuration complete\n"
#     conn.sendall(ack.encode())
#     conn.close()
#     server.close()
#     logging.info("[Tower Config] Tower configuration complete. MIC_POSITIONS updated:")
#     for mic in MIC_ORDER:
#         logging.info(f"   Mic {mic}: {MIC_POSITIONS[mic]}")

def tower_configuration_server():
    """
    Wait for a connection from the tablet.
    Securely exchange keys and send the mic indexes (MIC_ORDER).
    Then wait to receive three messages of the form:
        micIndex,x,y
    which will update MIC_POSITIONS.
    """
    global MIC_POSITIONS, TABLET_IP

    server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
    server.bind(('', TOWER_CONFIG_PORT))
    server.listen(1)

    logging.info(f"[Tower Config] Waiting for tower configuration connection on port {TOWER_CONFIG_PORT}...")
    conn, addr = server.accept()
    TABLET_IP = addr[0]
    logging.info(f"[Tower Config] Connected by {TABLET_IP}")

    # Perform RSA key exchange and receive AES key
    exchange_keys_with_tablet(conn)

    # Send the mic indexes securely
    mic_indexes_str = ",".join(str(m) for m in MIC_ORDER) + "\n"
    encrypted_mic_indexes = encrypt_message(mic_indexes_str)
    conn.sendall(encrypted_mic_indexes.encode())
    logging.info(f"[Tower Config] Sent encrypted mic indexes.")

    # Wait to receive encrypted tower coordinates
    received = {}
    while len(received) < len(MIC_ORDER):
        encrypted_data = conn.recv(1024)
        if not encrypted_data:
            break

        try:
            data = decrypt_message(encrypted_data.decode()).strip().splitlines()
            for line in data:
                parts = line.split(',')
                if len(parts) != 3:
                    continue

                mic_index = int(parts[0])
                x = float(parts[1])
                y = float(parts[2])
                received[mic_index] = (x, y)
                logging.info(f"[Tower Config] Received encrypted tower coordinate for mic {mic_index}: ({x}, {y})")
        except Exception as e:
            logging.info(f"[Tower Config] Error decrypting data: {e}")
            continue

    # Send acknowledgment securely
    ack = "Tower configuration complete\n"
    encrypted_ack = encrypt_message(ack)
    conn.sendall(encrypted_ack.encode())
    conn.close()
    server.close()
    logging.info("[Tower Config] Tower configuration complete.")
