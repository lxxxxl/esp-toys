
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_AHTX0.h>
#include "YandexDiskClient.h"

// Wi-Fi credentials
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASS"
#define AUTH_TOKEN "TOKEN"
#define DEVICE_PREFIX "test"

YandexDiskClient disk(AUTH_TOKEN);

// UTC+3 offset
const long utcOffsetInSeconds = 3 * 60 * 60;

// used to toggle LED status
uint8_t ledStatus = 0;

// used to deepsleep if hangs in my loops
uint8_t watchdogTimer;

// one hour in seconds
// hours * minutes * seconds
//#define ONE_HOUR (1 * 10)
#define ONE_HOUR (1 * 60 * 60)
// deepsleep time in seconds
#define SLEEP_TIME_SECONDS ONE_HOUR

// NTP client vars
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// RCT memory data struct
typedef struct {
  uint32_t signature;     // used to check validity of RTC data
  uint8_t count;
  uint16_t temperature[10];
  uint16_t humidity[10];
} RTCMem;
RTCMem rtcMem;

// AHT10 sensor related vars
Adafruit_AHTX0 aht;
sensors_event_t hum, temp;

void toggleLed()
{
  digitalWrite(2, ledStatus);
  ledStatus = !ledStatus;
}

/* Puts MCU to deepsleep state */
void deepsleep()
{
  Serial.println("Going to sleep...");
  digitalWrite(2, HIGH);
  // seconds * nanoseconds
  ESP.deepSleep(SLEEP_TIME_SECONDS * 1e6);
}

/* Connects to specified Wi-Fi network */
void wifiConnect()
{
  Serial.print("Connecting to AP");
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  watchdogTimer = 0;
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    toggleLed();
    delay(200);
    watchdogTimer++;
    if (watchdogTimer > 100)
      deepsleep();
  }
  Serial.println();
  Serial.println("WiFi connected.");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

/* Retrieves epoch time from NTP server */
unsigned long epochTime()
{
  timeClient.begin();
  watchdogTimer = 0;
  while (!timeClient.update()){
    toggleLed();
    delay(50);
    watchdogTimer++;
    if (watchdogTimer > 100)
      deepsleep();
  }
  return timeClient.getEpochTime();
}

/* Uploads temperature and humidity to Yandex Disk */
void uploadData()
{

  char buf[512];
  unsigned long time = epochTime();
  digitalWrite(2, LOW);

  strcpy(buf, "");
  for (int i = 0; i < rtcMem.count; i++){
    sprintf(buf, "%s\n%hu;%hu", buf, rtcMem.temperature[i], rtcMem.humidity[i]);
  }
  
  String filename = "/test/" DEVICE_PREFIX "/" + String(time) + ".csv";
  disk.upload(filename, (uint8_t*)&buf, strlen(buf));
}

/* Polls AHT10 sensor */
void pollAHT10(float* temperature, float* humidity)
{
  watchdogTimer = 0;
  while (!aht.begin()){
    toggleLed();
    delay(10);
    watchdogTimer++;
    if (watchdogTimer > 100)
      deepsleep();
  }
  Serial.println("Found AHT10");

  aht.getEvent(&hum, &temp);// populate temp and humidity objects with fresh data
  *temperature = temp.temperature;
  *humidity = hum.relative_humidity;
}

/* saves temperature and humidity to Yandex Disk */
void saveTemperatureAndHumidity()
{
  float temperature, humidity;
  pollAHT10(&temperature, &humidity);
  Serial.print("Temp: ");
  Serial.print(temperature);
  Serial.println();
  Serial.print("Humidity: ");
  Serial.print(humidity);
  Serial.println();
  rtcMem.temperature[rtcMem.count] = round(temperature * 100);
  rtcMem.humidity[rtcMem.count] = round(humidity * 100);
  rtcMem.count++;
  
}

void setup()
{
  Serial.begin(9600);
  pinMode(2, OUTPUT);

  ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcMem, sizeof(rtcMem));
  
  // check validity of RTC data
  if ((rtcMem.signature != 0xCAFEF00D) || (rtcMem.count > 10)){
    rtcMem.signature = 0xCAFEF00D;
    rtcMem.count = 0;
  }

  // upload saved temperature data every 10 cycles
  Serial.printf("Wake up times: %d\n", rtcMem.count);
  if ((rtcMem.count == 10)){
    wifiConnect();
    uploadData();
    rtcMem.count = 0;
  }

  saveTemperatureAndHumidity();
  ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcMem, sizeof(rtcMem));
  deepsleep();
}

void loop() 
{
}
