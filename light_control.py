from machine import Pin, ADC, Timer
from machine import RTC, deepsleep

def goto_sleep(delay):
    ''' sends device to deep sleep for `delay` seconds '''
    # configure RTC.ALARM0 to be able to wake the device
    rtc = machine.RTC()
    rtc.irq(trigger=rtc.ALARM0, wake=machine.DEEPSLEEP)
    # set RTC.ALARM0 to fire after delay seconds (waking the device)
    rtc.alarm(rtc.ALARM0, delay * 1000)
    # put the device to sleep
    #machine.deepsleep()

# lightness treshold
treshold = 300
# time for which we should turn on lights, seconds
activity_time = 60 * 30     # 30 minutes
# inactivity time after turning lights off, seconds
inactivity_time = 60 * 60 * 8       # 8 hours
# create ADC object to poll photoresistor
photoresistor_pin = ADC(0)
# pin with led's +
light_pin = Pin(4, Pin.OUT, value=0)

# poll photoresistor
lightness = photoresistor_pin.read()
new_val = lightness < treshold
if new_val:
    light_pin.value(1)
    timer = Timer(-1)
    timer.init(period=activity_time*1000, mode=Timer.ONE_SHOT, callback=lambda t: goto_sleep(inactivity_time))
else:
    goto_sleep(5)

