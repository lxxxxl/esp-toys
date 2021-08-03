// HC-SR04 pins
#define PIN_TRIG 12
#define PIN_ECHO 14
// speaker + pin
#define PIN_SPEAKER 15

// ultrasonic echo timing ranges
#define MIN_DURATION 100
#define MAX_DURATION 4000
// frequency ranges for tone
#define MIN_FREQ 30
#define MAX_FREQ 1700


void setup() {
  Serial.begin (115200);
  pinMode(PIN_TRIG, OUTPUT);
  pinMode(PIN_ECHO, INPUT);
  pinMode(PIN_SPEAKER, OUTPUT);
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

  // do nothing if duration > MAX_DURATION because it is too far
  if (duration > MAX_DURATION){
    delay(100);
    return;
  }

  // convert duration to frequency
  duration = MAX_DURATION - duration;
  float scale = (float)(MAX_FREQ - MIN_FREQ) / (float)(MAX_DURATION - MIN_DURATION);
  int freq = MIN_FREQ + ((duration - MIN_DURATION) * scale);

  // debug print
  Serial.print(freq);
  Serial.println(" Hz");

  tone(PIN_SPEAKER, freq, 200);
}
