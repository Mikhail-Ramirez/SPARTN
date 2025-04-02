import pywifi
import time
import wifi_scan

ssid_list = wifi_scan.wifi_scan()

for ssid in ssid_list:
    print(ssid)