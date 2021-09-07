
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include <Adafruit_AHTX0.h>
#include <SD.h>

// Wi-Fi credentials
#define WIFI_SSID "SSID"
#define WIFI_PASSWORD "PASSWORD"

// UTC+3 offset
const long utcOffsetInSeconds = 3 * 60 * 60;

// one hour in seconds
// hours * minutes * seconds
#define ONE_HOUR (1 * 60 * 60)
// deepsleep time in seconds
#define SLEEP_TIME_SECONDS ONE_HOUR

// NTP client vars
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org", utcOffsetInSeconds);

// RCT memory data struct
typedef struct {
  unsigned long epochTime;
  uint8_t resetCounter;
} RTCMem;
RTCMem rtcMem;

// AHT10 sensor related vars
Adafruit_AHTX0 aht;
sensors_event_t hum, temp;

// SD card related vars
#define SD_CS 15
#define SD_FILENAME "data.csv"
File file;


/* Connects to specified Wi-Fi network */
void wifiConnect()
{
  Serial.print("Connecting to AP");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED){
    Serial.print(".");
    delay(200);
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
  while (!timeClient.update())
    delay(50);
  return timeClient.getEpochTime();
}

/* Puts MCU to deepsleep state */
void deepsleep()
{
  Serial.println("Going to sleep...");
  // seconds * nanoseconds
  ESP.deepSleep(SLEEP_TIME_SECONDS * 1e6);
}

/* Initializes SD card and dest file */
void SDInit()
{
  if (!SD.begin(SD_CS)) {
    Serial.println("SD initialization failed!");
    return;
  }
  Serial.println("SD initialization done.");
  if (SD.exists(SD_FILENAME)) {
    Serial.println("File exists.");
  } else {
    Serial.println("File exist, creating...");
  }
  file = SD.open(SD_FILENAME, FILE_WRITE);
}

/* Writes temperature and humidity to file on SD card */
void SDFileWrite(float temperature, float humidity)
{
  file.printf("%lu;%f;%f\n", rtcMem.epochTime, temperature, humidity);
  file.close();
  SD.end();
}

/* Polls AHT10 sensor */
void pollAHT10(float* temperature, float* humidity)
{
  while (!aht.begin())
    delay(10);
  Serial.println("Found AHT10");

  aht.getEvent(&hum, &temp);// populate temp and humidity objects with fresh data
  *temperature = temp.temperature;
  *humidity = hum.relative_humidity;
}

/* saves temperature and humidity to SD card */
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
  SDFileWrite(temperature, humidity);
}

void setup()
{
  Serial.begin(9600);
  ESP.rtcUserMemoryRead(0, (uint32_t*) &rtcMem, sizeof(rtcMem));
  // reset saved time every 10 deepsleeps
  Serial.printf("RTC Data: %d %ld\n", rtcMem.resetCounter, rtcMem.epochTime);
  if (rtcMem.resetCounter == 0 || rtcMem.resetCounter >= 10 || rtcMem.epochTime == 0){
    wifiConnect();
    unsigned long time = epochTime();
    // wait for start of hour to perform measurements at the start of hour
    uint16_t waitTime = ONE_HOUR - time % ONE_HOUR;
    Serial.printf("Current time: %ld, waiting %d more\n", time % ONE_HOUR, waitTime);
    rtcMem.resetCounter = 1;
    rtcMem.epochTime = time + waitTime;
    ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcMem, sizeof(rtcMem));
    if (waitTime > 0){
      Serial.println("Waiting for new hour...");
      ESP.deepSleep(waitTime * 1e6);
    }
  }
  else{
    Serial.print("Wake up times: ");
    Serial.print(rtcMem.resetCounter);
    Serial.println();
    rtcMem.epochTime += 3;  // assume that we are working 3 seconds
    rtcMem.epochTime += SLEEP_TIME_SECONDS;
    rtcMem.resetCounter += 1;
  }
  SDInit();
  ESP.rtcUserMemoryWrite(0, (uint32_t*) &rtcMem, sizeof(rtcMem));
  saveTemperatureAndHumidity();
  deepsleep();
}

void loop() 
{
}