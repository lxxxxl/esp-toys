import network
import machine

def ap_to_str(ap):
    '''Converts AP info to string format:
    NAME (BSSID) AUTH_MODE STATUS chan:CHANNEL RSSI'''
    mac_addr = '%02X:%02X:%02X:%02X:%02X:%02X' % (ap[1][0], ap[1][1], ap[1][2], ap[1][3], ap[1][4], ap[1][5])
    auth_mode = 'OPEN'
    if ap[4] == 1: auth_mode = 'WEP'
    elif ap[4] == 2: auth_mode = 'WPA-PSK'
    elif ap[4] == 3: auth_mode = 'WPA2-PSK'
    elif ap[4] == 3: auth_mode = 'WPA/WPA2-PSK'
    status = 'visible'
    if not ap[5]:
        status = 'hidden'

    return '%s (%s) %s %s chan:%d %ddBm' % (str(ap[0].decode('utf-8')), mac_addr, auth_mode, status, ap[2], ap[3])

def ap_list():
    '''Shows currently visible APs'''
    global nic
    scan_results = nic.scan()

    print('Currently visible APs:')
    for result in scan_results:
        print(ap_to_str(result))

def ap_find(ap_name):
    '''Starts AP searching'''
    print('Searching %s...' % ap_name)
    global timer
    timer.init(period=5000, mode=machine.Timer.PERIODIC, callback=lambda t: timer_task(ap_name))

def ap_find_stop():
    '''Stops AP searching'''
    global timer
    print('Stopping search...')
    timer.deinit()

def timer_task(ap_name):
    '''Performs one search iteration
    Finds all APs and changes beep tone if there is needed AP'''
    global nic
    scan_results = nic.scan()
    ap_name = ap_name.encode('utf-8')
    for result in scan_results:
        if result[0]== ap_name:
            print(ap_to_str(result))

# enable station interface
nic = network.WLAN(network.STA_IF)
nic.active(True)
# create timer object
timer = machine.Timer(-1)

print('Type "ap_list()" to get list of available APs')
print('Type "ap_find(AP_NAME)" to start searching AP.')
print('Type "ap_find_stop()" to stop search.')

