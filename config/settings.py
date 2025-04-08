# config/settings.py
import numpy as np
from datetime import datetime


SAMPLE_RATE = 48000
WINDOW_DURATION = 1.0
CHUNK_DURATION = 0.1
MIC_ORDER = [2, 3, 4]   # Add 4th ALSA index  

CHANNELS = 1               # Mono recording
SPEED_OF_SOUND = 343         # m/s

# Microphone positions (ALSA card numbers → Physical positions)
MIC_POSITIONS = {
    # 1: np.array([0, 0]), # Need to get the correct ALSA index for the 4th mic 
    2: np.array([0, 0]),# Mic2 
    3: np.array([0, 0]),# Mic3
    4: np.array([0, 0]) # Mic4
}

# CSV logging file
log_time = datetime.now().strftime("%Y-%m-%d.%H.%M")
LOG_FILE =f"data/{log_time}_log.log"


# TCP Ports:
#   39440 is used for tower configuration (bi-directional exchange with the tablet)
#   39439 is used to send live (x,y) updates to the tablet.
TOWER_CONFIG_PORT = 39440

# This will be set from the incoming tower configuration connection (i.e. the tablet's IP)
TABLET_IP = "10.4.153.44" 
