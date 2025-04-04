# communications/tablet_comm.py
from config.settings import *
import logging
import socket

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
        message = f"location,{x:.2f},{y:.2f}\n" # verify this now works
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent location: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending location: {e}")


def send_classification(result):
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
        message = f"classification,{result}\n" #verify this
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent classification: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending location: {e}")
