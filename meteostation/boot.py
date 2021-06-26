import network
import esp
import gc

esp.osdebug(None)
gc.collect()

ssid = 'YOUR_SSID'
password = 'YOUR_PASSWORD'

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
  pass


print('Wi-Fi Connection successful')
print(station.ifconfig())
