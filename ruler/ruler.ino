#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

// HC-SR04 pins
#define PIN_TRIG 12
#define PIN_ECHO 14

// Wi-Fi AP settings
const char* ssid = "ruler";
const char* password = "password";

// Broadcast IP address and port;
WiFiUDP udp;
IPAddress broadcast = IPAddress(192, 168, 4, 255);
const int port = 8811;

void setup() {
  Serial.begin (115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);

  Serial.println();
  Serial.print("Configuring access point...");
  WiFi.softAP(ssid, password);

  IPAddress myIP = WiFi.softAPIP();
  Serial.print("AP IP address: ");
  Serial.println(myIP);
  
  udp.begin(port);
}
 
void loop() {
  int duration, distance;
  // send ultasonic signal
  digitalWrite(PIN_TRIG, LOW);
  delayMicroseconds(2);
  digitalWrite(PIN_TRIG, HIGH);
  // wait for 10ns
  delayMicroseconds(10);
  digitalWrite(PIN_TRIG, LOW);
  
  // get duration of HIGH on PIN_ECHO
  duration = pulseIn(PIN_ECHO, HIGH);
  // calculate distance
  // distance_two_way = duration * 340 m/s = duration * 0.034 m/mks = duration * 1/29
  // distance = distance_two_way / 2
  distance = duration / 58;
  // debug print
  Serial.print(distance);
  Serial.println(" cm");

  // broadcast distance via UDP
  udp.beginPacket(broadcast,port);
  udp.write((char*)&distance, 4);
  udp.endPacket();
  
  delay(100);
}
