import pywifi
import time

#This program automatically scans for all SSIDs and returns a list.

def wifi_scan():
    wifi = pywifi.PyWiFi()
    iface = wifi.interfaces()[0]
    iface.scan()
    time.sleep(0.5)
    results = iface.scan_results()

    ssid_list = []

    for i in results:
        bssid = i.bssid
        ssid  = i.ssid
        ssid_list.append(ssid)

    ssid_list = list(set(ssid_list))

    ssid_list.sort()

    return ssid_list