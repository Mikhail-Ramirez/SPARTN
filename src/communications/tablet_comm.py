# communications/tablet_comm.py
from config.settings import *
import logging
import socket
from encryption import perform_handshake_send, encrypt_message

def send_location(x, y):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y),
    using RSA/AES handshake and AES encryption.
    """
    global TABLET_IP
    if TABLET_IP is None:
        logging.info("[Send] Tablet IP not configured; cannot send location.")
        return
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TABLET_IP, 39439))
        # Perform handshake as sender.
        perform_handshake_send(sock)
        message = f"location,{x:.2f},{y:.2f}\n"
        encrypted = encrypt_message(message)
        sock.sendall(encrypted)
        sock.close()
        logging.info(f"[Send] Sent encrypted location: {message.strip()}")
    except Exception as e:
        logging.error(f"[Send] Error sending location: {e}")

def send_classification(result):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the classification result,
    using RSA/AES handshake and AES encryption.
    """
    global TABLET_IP
    if TABLET_IP is None:
        logging.info("[Send] Tablet IP not configured; cannot send classification.")
        return
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TABLET_IP, 39439))
        perform_handshake_send(sock)
        message = f"classification,{result}\n"
        encrypted = encrypt_message(message)
        sock.sendall(encrypted)
        sock.close()
        logging.info(f"[Send] Sent encrypted classification: {message.strip()}")
    except Exception as e:
        logging.error(f"[Send] Error sending classification: {e}")

