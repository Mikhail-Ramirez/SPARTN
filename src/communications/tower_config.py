# communications/tower_config.py
import socket
import logging
import time
import numpy as np
from config.settings import TOWER_CONFIG_PORT, MIC_ORDER, TABLET_IP, MIC_POSITIONS  # Ensure these are defined

from encryption import perform_handshake_receive, encrypt_message, decrypt_message

def configuration_complete():
    complete = all(
        (pos[0] is not None and pos[1] is not None) for pos in MIC_POSITIONS.values()
    )
    return complete

def process_connection(conn):
    """Process messages from one configuration connection with RSA/AES handshake."""
    global TABLET_IP
    try:
        # Perform handshake as receiver (tablet is sender)
        perform_handshake_receive(conn)
        # Now receive the encrypted configuration messages.
        data = conn.recv(4096)
        if not data:
            return
        # Decrypt the received message (expected to be plaintext CSV lines originally)
        decrypted_data = decrypt_message(data)
        lines = decrypted_data.strip().splitlines()
        for line in lines:
            # Look for the initial handshake message.
            if line.startswith("configHandshake"):
                parts = line.split(',')
                if len(parts) >= 2:
                    TABLET_IP = parts[1]
                    logging.info(f"[Tower Config] Received config handshake from {TABLET_IP}")
                    # Immediately send the mic indexes to the tablet (encrypted).
                    mic_indexes_str = ','.join(str(m) for m in MIC_ORDER)
                    response = encrypt_message(mic_indexes_str + "\n")
                    conn.sendall(response)
                    logging.info(f"[Tower Config] Sent mic indexes: {mic_indexes_str}")
            # Process "sendLocation" messages which update a specific tower’s coordinate.
            elif line.startswith("sendLocation"):
                # Expected format: sendLocation,<micIndex>,<x>,<y>
                parts = line.split(',')
                if len(parts) != 4:
                    logging.info(f"[Tower Config] Invalid sendLocation format: {line}")
                    continue
                try:
                    mic_index = int(parts[1])
                    x = float(parts[2])
                    y = float(parts[3])
                    MIC_POSITIONS[mic_index] = np.array((x, y))
                    logging.info(f"[Tower Config] Updated tower coordinate for mic {mic_index}: ({x}, {y})")
                except Exception as e:
                    logging.info(f"[Tower Config] Error parsing sendLocation line '{line}': {e}")
            # Process initial coordinate messages: "micIndex,x,y"
            else:
                parts = line.split(',')
                if len(parts) != 3:
                    logging.info(f"[Tower Config] Skipping invalid format: {line}")
                    continue
                try:
                    mic_index = int(parts[0])
                    x = float(parts[1])
                    y = float(parts[2])
                    MIC_POSITIONS[mic_index] = np.array((x, y))
                    logging.info(f"[Tower Config] Received tower coordinate for mic {mic_index}: ({x}, {y})")
                except Exception as e:
                    logging.info(f"[Tower Config] Error parsing line '{line}': {e}")
    except Exception as e:
        logging.info(f"[Tower Config] Error during connection processing: {e}")
    finally:
        conn.close()

def tower_configuration_server():
    """
    Wait for the tablet’s configuration messages until all towers have been configured.
    The server accepts a connection, performs a handshake, then processes encrypted messages.
    """
    global MIC_POSITIONS
    logging.info(f"[Tower Config] Starting tower configuration server on port {TOWER_CONFIG_PORT}...")

    while not configuration_complete():
        try:
            server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server.bind(('', TOWER_CONFIG_PORT))
            server.listen(1)
            logging.info(f"[Tower Config] Waiting for configuration connection on port {TOWER_CONFIG_PORT}...")
            conn, addr = server.accept()
            logging.info(f"[Tower Config] Connection from {addr[0]}")
            process_connection(conn)
        except Exception as e:
            logging.info(f"[Tower Config] Error accepting connection: {e}")
        finally:
            server.close()
            if not configuration_complete():
                logging.info("[Tower Config] Tower configuration not complete. Waiting for next connection...")
                time.sleep(1)

    logging.info("[Tower Config] Tower configuration complete. MIC_POSITIONS updated:")
    for mic in MIC_ORDER:
        logging.info(f"   Mic {mic}: {MIC_POSITIONS[mic]}")

