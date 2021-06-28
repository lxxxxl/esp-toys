import network
import socket

WWW_ROOT = '/www'

def file_exists(filename):
    '''checks if file exists'''
    try:
        f = open(filename, "r")
        f.close()
        return True
    except OSError:
        return False

def extract_filename(request):
    '''extracts requested filename from HTTP request'''
    prefix = 'GET '
    suffix = ' HTTP/1'
    pos_start = request.find(prefix)
    # return None if we cannot find prefix
    if pos_start == -1:
        return None
    pos_start += len(prefix)

    pos_end = request.find(suffix)
    # return None if we cannot find suffix
    if pos_end == -1:
        return None
    
    return request[pos_start : pos_end]

def process_filename(filename):
    '''perform some mutations on filename'''
    global WWW_ROOT

    # remove potential dangerous constructions
    filename = filename.replace('..','')
    filename = filename.replace('|','')
    filename = filename.replace('>','')
    filename = filename.replace('<','')

    # add index.html if we are requesting directory root
    if filename.endswith('/'):
        filename += 'index.html'

    # add root directory name to path
    if not filename.startswith(WWW_ROOT):
        filename = WWW_ROOT + filename
    
    return filename



# connect to Wi-Fi AP
ssid = 'YOUR_SSID'
password = 'YOUR_PASSWORD'

station = network.WLAN(network.STA_IF)
station.active(True)
station.connect(ssid, password)

while station.isconnected() == False:
    pass

print('Wi-Fi Connection successful')
print(station.ifconfig())

# create HTTP socket
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind(('', 80))
s.listen(5)

# process connections
while True:
    conn, addr = s.accept()
    request = conn.recv(1024)
    request = str(request)
    filename = extract_filename(request)
    if not filename:
        continue

    print('Requested %s from %s\n' % (filename, str(addr)))

    filename = process_filename(filename)

    if not file_exists(filename):
        print('file not exists')
        conn.send('HTTP/1.1 404\n\n')
        conn.close()
        continue

    response = None
    with open(filename,'r') as file:
        response = file.read()

    conn.send('HTTP/1.1 200 OK\n')
    conn.send('Content-Type: text/html\n')
    conn.send('Connection: close\n\n')
    conn.sendall(response)
    conn.close()