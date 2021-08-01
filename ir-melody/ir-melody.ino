#include <IRremote.h>
#include "melodies.h"

// PIN definitions
#define IR_SENSOR 13
#define SPEAKER 3

#define NOTE_DELAY 100
#define COMMAND_NOTE_MULTIPIER 50

#define NOTE_SLEEP 10
#define DEBUG_SERIAL

int command;
byte command_counter;


song *melodies[] = {&UpDown, &HappyBirthday, &Chizhick, &Gusi, &JingleBells, &SpyatUstalieIgrushki, &Elochka};
short notes[] = {NOTE_A, NOTE_B, NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G};

void setup() {
  randomSeed(-1);

  command_counter = 0;
  
  pinMode(SPEAKER, OUTPUT);
  pinMode(IR_SENSOR, INPUT);
  
  IrReceiver.begin(IR_SENSOR);

  //RandomMelody();

#if defined(DEBUG_SERIAL)
  Serial.begin(9600);
#endif
}

void loop() {
  if (IrReceiver.decode()) {
#if defined(DEBUG_SERIAL)
    IrReceiver.printIRResultMinimal(&Serial);
#endif  

    command = IrReceiver.decodedIRData.command;
    if (command != 0) {
      IrReceiver.stop();
  
      int _tone = IrReceiver.decodedIRData.command * COMMAND_NOTE_MULTIPIER;
      note(_tone, NOTE_DELAY, NOTE_SLEEP);

      // play predefined melody after 10 keypresses
      command_counter++;
      if (command_counter > 20){
        RandomMelody();
        command_counter = 0;  
      }

      // play random melody in 2% of keypresses
      if (command_counter && random(0, 100) <= 2)
        GenerateRandomMelody();

      IrReceiver.start(8000); // to compensate for 11 ms stop of receiver. This enables a correct gap measurement.
    
    }
    IrReceiver.resume();
  }
  delay(50);
}

// play random melody from predefined melodies
void RandomMelody(){
  int i = random(0, PREDEFINED_SONGS_COUNT);
  play(melodies[i]);
}

// generate random melody
// melody structure is: 
// X notes    [ac|dec]cending
// X notes    [dec|ac]cending
// X+4 notes  [ac|dec]cending
// 3 notes    [dec|ac]cending
void GenerateRandomMelody() {
  int notes_count = random(3, 8);
  bool direction = random(0,2);
  int total_notes = 0;

  memset(&Random, 0, sizeof(song));
  
  GenerateRandomChord(notes_count, direction, total_notes);
  total_notes += notes_count;
  GenerateRandomChord(notes_count, direction - 1, total_notes);
  total_notes += notes_count;
  GenerateRandomChord(notes_count + 4, direction, total_notes);
  total_notes += notes_count + 4;
  GenerateRandomChord(3, direction -1, total_notes);  


  play(&Random);
}

// generate chord for random melody
void GenerateRandomChord(int notes_count, int direction, int pos) {
  // take as first note A, B or C if accesnidg, else E, F, or G
  int _note = direction ? random(0,3) : random(5, 7);
  // set note delay == previous_note_delay +- [0,100]
  int delay = 0;
  for (int i = 0; i < notes_count; i++){
    Random.length++;
    Random.notes[i+pos] = notes[_note];

    if ((i + pos) == 0) 
      delay = random(1, 5) * 50;
    else
      delay = Random.delays[i+pos-1] + random(-1, 2) * 50;
    if (delay <= 0)
      delay = random(1, 5) * 50;
    Random.delays[i+pos] = delay;

    // take next note according to direction
    _note += direction ? 1 : -1;
    // take previous note if we are out of borders
    if (_note > 6)
      _note = 5;
    if (_note < 0)
      _note = 1;
  }
}

// play one note
void note(int _tone, int _delay, int _sleep) {
  if (_tone != 0)
    tone(SPEAKER, _tone);
  delay(_delay);
  noTone(SPEAKER);
  delay(_sleep);
}

// play melody from pattern
void play(song *_song){
  for (int i = 0; i < _song->length; i++) {
    note( _song->notes[i], _song->delays[i], NOTE_SLEEP);
  }
}



