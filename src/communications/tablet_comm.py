# communications/tablet_comm.py
from config.settings import *
from communications.encryption import rsa_encrypt, encrypt_message
import socket
import logging
from Crypto.Random import get_random_bytes

# def send_location(x, y):
#     """
#     Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y).
#     """
#     global TABLET_IP
#     if TABLET_IP is None:
#         logging.info("[Send] Tablet IP not configured; cannot send location.")
#         return
#     try:
#         sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
#         sock.connect((TABLET_IP, 39439))
#         message = f"{x:.2f},{y:.2f}\n"
#         sock.sendall(message.encode())
#         sock.close()
#         logging.info(f"[Send] Sent location: {message.strip()}")
#     except Exception as e:
#         logging.info(f"[Send] Error sending location: {e}")

def send_location(x, y):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y).
    """
    global TABLET_IP
    if TABLET_IP is None:
        logging.info("[Send] Tablet IP not configured; cannot send location.")
        return
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((TABLET_IP, 39439))

        # Receive RSA public key from the tower
        rsa_public_key = sock.recv(450)  # Approximate size for a 2048-bit key
        logging.info("[Send] Received RSA public key.")

        # Generate a random AES key and encrypt it with RSA
        aes_key = get_random_bytes(16)
        encrypted_aes_key = rsa_encrypt(aes_key, rsa_public_key)
        sock.sendall(encrypted_aes_key)
        logging.info("[Send] AES Key sent securely.")

        # Encrypt location message with AES
        message = f"{x:.2f},{y:.2f}\n"
        encrypted_message = encrypt_message(message)

        # Send encrypted message
        sock.sendall(encrypted_message.encode())
        sock.close()
        logging.info(f"[Send] Sent encrypted location: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending location: {e}")
