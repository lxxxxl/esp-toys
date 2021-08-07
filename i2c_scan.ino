#include <Wire.h>
 
void setup(){
  byte error, address;
  int nDevices;
  Wire.begin();    
  Serial.begin(115200);
 
  Serial.println("Scanning for I2C devices...");
 
  for(address = 8; address < 127; address++ ){
    Wire.beginTransmission(address);
    error = Wire.endTransmission();
 
    if (error == 0){
      Serial.print("I2C device found at address 0x");
      if (address < 16)
        Serial.print("0");
      Serial.print(address,HEX);
      Serial.println();
    }
  }
    
  Serial.println("Done");
} 
 
void loop(){
}
