from machine import Pin, I2C

from micropyserver import MicroPyServer
from ahtx0 import AHT10

def return_main(request):
    ''' main request handler '''
    global sensor

    html_head = '<html><head><meta charset="utf-8"></head><body><table>'
    html_tail = '</table></body></html>'
    html = '<tr><td><img src=\'/thermometer.png\' /></td><td><font size=\'64px\'>' + str(int(sensor.temperature)) + '°</font></td></tr>' \
          '<tr><td><img src=\'/humidity.png\' /></td><td><font size=\'64px\'>' + str(int(sensor.relative_humidity)) + '%</font></td></tr>' \
          '<script>' \
          'setTimeout(function(){' \
          '  window.location.reload(1);' \
          '}, 10000);' \
          '</script>'
    

    server.send("HTTP/1.0 200 OK\r\n")
    server.send("Content-Type: text/html\r\n\r\n")
    
    server.send(html_head)
    server.send(html)
    server.send(html_tail)

def return_icon_humidity(request):
    server.send("HTTP/1.0 200 OK\r\n")
    server.send("Content-Type: image/png\r\n\r\n")
    with open("/humidity.png", "rb") as img:
        data = img.read()
        server.send_raw(data)

def return_icon_temperature(request):
    server.send("HTTP/1.0 200 OK\r\n")
    server.send("Content-Type: image/png\r\n\r\n")
    with open("/thermometer.png", "rb") as img:
        data = img.read()
        server.send_raw(data)
    
i2c = I2C(scl=Pin(5), sda=Pin(4))
sensor = AHT10(i2c)

server = MicroPyServer()
''' add route '''
server.add_route("/", return_main)
server.add_route("/humidity.png", return_icon_humidity)
server.add_route("/thermometer.png", return_icon_temperature)
''' start server '''
server.start()
