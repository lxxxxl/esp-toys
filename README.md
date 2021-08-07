# esp-toys
Toy projects based on Espressif boards

## NodeMCU v3 pin numbering for MicroPython and Arduino
![image_1](https://raw.githubusercontent.com/lxxxxl/esp-toys/master/images/pins.png?raw=true)  

## Light control
Controls small LED strip according to current lightness.  
1. Connect photoresistor to ADC PIN.  
2. Connect LED+ to GPIO4 PIN.  

## Wi-Fi AP Search
Continuously shows RSSI level of selected AP.  
Type `ap_list()` to get list of available APs.  
Type `ap_find(AP_NAME)` to start searching AP.  
Type `ap_find_stop()` to stop search.  

## Meteostation
Shows temperature and humidity.  
Dependencies:  
- [MicroPyServer, a simple HTTP server for MicroPython projects](https://github.com/troublegum/micropyserver)  
- [MicroPython driver for the AHT10 and AHT20 temperature and humindity sensors](https://github.com/targetblank/micropython_ahtx0)  

## HTTP Server
Static HTTP server with HTML-only support.  
1. Place your html files to `/www` directory.  
2. Replace SSID and PASSWORD with yours.  

## Mail Alert
Sends email on some alert.  
Connect alert signal source to RST pin. Source should emit LOW signal on some event.  
Uses [ESP Mail Client](https://github.com/mobizt/ESP-Mail-Client) library.

## IR Melody
Turns TV remote to piano.  
Sometimes plays one of 7 predefined melodies, or generates and plays a new one.  
Uses `VS1838 IR sensor`.  
Pinout (front view):  
| OUT | GND | VCC |

## Ruler
Calculates distance to objects using `HC-SR04` ultrasonic sensor.  
`ruler.ino` calculates distance and broadcasts it via UDP.  
`app` shows distance in UI.

## Termenvox
[Theremin](https://en.wikipedia.org/wiki/Theremin) implementation based on `HC-SR04` ultrasonic sensor.  

## I2C Scan
Scans I2C bus for devices.  
