import subprocess,re
r=subprocess.run(['sudo','iwlist','wlan0','scan'],capture_output=True,text=True).stdout
s=re.findall(r'ESSID:"(.*?)"',r)
print("Available Wi-Fi Networks:\n"+"\n".join(f"{i+1}. {s[i]}" for i in range(len(s))) if s else "No networks found.")
