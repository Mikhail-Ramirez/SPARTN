# communications/tower_config.py
import socket
import logging
import time
import numpy as np
from multiprocessing import Manager

from config.settings import TOWER_CONFIG_PORT, MIC_ORDER, MIC_POSITIONS  # Keep importing from settings

# Use a manager to share TABLET_IP across processes
def setup_manager():
    manager = Manager()
    TABLET_IP = manager.Value('s', None)  # Shared string for TABLET_IP
    return TABLET_IP

def configuration_complete(TABLET_IP):
    """Check if the tablet IP is set."""
    if TABLET_IP.value is not None:
        return True
    return False

def process_connection(conn, TABLET_IP):
    """Process messages from one configuration connection."""
    try:
        data = conn.recv(1024)
        if not data:
            return
        lines = data.decode().strip().splitlines()
        for line in lines:
            if line.startswith("configHandshake"):
                parts = line.split(',')
                if len(parts) >= 2:
                    TABLET_IP.value = parts[1]  # Update the shared TABLET_IP
                    logging.info(f"[Tower Config] Received config handshake from {TABLET_IP.value}")
    except Exception as e:
        logging.info(f"[Tower Config] Error during connection processing: {e}")
    finally:
        conn.close()


def tower_configuration_server(TABLET_IP):
    """Wait for the tablet’s configuration messages until all towers have been configured."""
    logging.info(f"[Tower Config] Starting tower configuration server on port {TOWER_CONFIG_PORT}...")

    while not configuration_complete(TABLET_IP):
        try:
            server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
            server.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
            server.bind(('', TOWER_CONFIG_PORT))
            server.listen(1)
            logging.info(f"[Tower Config] Waiting for configuration connection on port {TOWER_CONFIG_PORT}...")
            conn, addr = server.accept()
            logging.info(f"[Tower Config] Connection from {addr[0]}")
            process_connection(conn, TABLET_IP)

        except Exception as e:
            logging.error(f"[Tower Config] Error accepting connection: {e}")
            time.sleep(1)  # Retry after a short delay in case of failure
        finally:
            server.close()

        if not configuration_complete(TABLET_IP):
            logging.info("[Tower Config] Tower configuration not complete. Waiting for next connection...")

    logging.info(f"[Tower Config] Tower configuration complete. Tablet IP is now set to {TABLET_IP.value}")
