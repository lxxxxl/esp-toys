# esp-toys
Toy projects based on Espressif boards

## Light control
Controls small LED strip according to current lightness.  
1. Connect photoresistor to ADC PIN.  
2. Connect LED+ to GPIO4 PIN.  

### NodeMCU PIN numbering in Micropython
| Micropython | Board                      |
|-------------|----------------------------|
| 0           | D3                         |
| 2           | D4 (also Led1 but inverse) |
| 4           | D2                         |
| 5           | D1                         |
| 9           | SD2                        |
| 10          | SD3                        |
| 12          | D6                         |
| 13          | D7                         |
| 14          | D5                         |
| 15          | D8                         |
| 16          | D0 (also Led2 but inverse) |

[Source](https://forum.micropython.org/viewtopic.php?p=21483&sid=3b1a4d930261549b9694cbeb5a9e03d3#p21483)  
  
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