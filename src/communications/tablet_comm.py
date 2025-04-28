# communications/tablet_comm.py
import logging
import socket
import time


def send_location(ip, x, y):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y).
    """
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ip, 39439))
        message = f"location,{x:.2f},{y:.2f}\n" # verify this now works
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent location: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending location: {e}")

def send_quadrant(ip, q):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated quadrant
    """
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ip, 39439))
        message = f"quadrant,{q}\n"
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent quadrant: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending quadrant: {e}")

def send_shift_values(ip, shift_values):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the shift percentage values
    The tablet app will receive the ordered list of shift percentages: [mic2, mic3, mic4, mic5]
    From there the tablet app will shift the red dot relative to the coordinate plane.
    """
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ip, 39439))
        message = f"shift values,{shift_values[0]:.6f},{shift_values[1]:.6f},{shift_values[2]:.6f},{shift_values[3]:.6f}\n"
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent Shift Values: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending shift values: {e}")

def send_classification(ip, result):
    """
    Opens a TCP connection to the tablet (TABLET_IP) on port 39439 to send the estimated (x,y).
    """
    try:
        sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        sock.connect((ip, 39439))
        message = f"classification,{result}\n" #verify this
        sock.sendall(message.encode())
        sock.close()
        logging.info(f"[Send] Sent classification: {message.strip()}")
    except Exception as e:
        logging.info(f"[Send] Error sending location: {e}")
