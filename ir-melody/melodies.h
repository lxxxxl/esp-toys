
#define NOTES_LOWER_BORDER  400
#define NOTES_UPPER_BORDER  1000
#define NOTES_GAP (NOTES_UPPER_BORDER - NOTES_LOWER_BORDER) / 6


#define NOTE_0 0
#define NOTE_A NOTES_LOWER_BORDER + NOTES_GAP * 0 // 1
#define NOTE_B NOTES_LOWER_BORDER + NOTES_GAP * 1 // 2
#define NOTE_C NOTES_LOWER_BORDER + NOTES_GAP * 2 // 3
#define NOTE_D NOTES_LOWER_BORDER + NOTES_GAP * 3 // 4
#define NOTE_E NOTES_LOWER_BORDER + NOTES_GAP * 4 // 5
#define NOTE_F NOTES_LOWER_BORDER + NOTES_GAP * 5 // 6
#define NOTE_G NOTES_LOWER_BORDER + NOTES_GAP * 6 // 7


#define MAX_MELODY_LENGTH 28
struct song {
  byte length;
  unsigned short notes[MAX_MELODY_LENGTH];
  unsigned short delays[MAX_MELODY_LENGTH];
};


#define PREDEFINED_SONGS_COUNT 7

song UpDown = {
  13,
  {NOTE_A, NOTE_B, NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_G, NOTE_F, NOTE_E, NOTE_D, NOTE_C, NOTE_B, NOTE_A, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {200,    200,    200,    200,    200,    200,    400,    200,    200,    200,    200,    200,    200,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

song HappyBirthday = {
  25,
  {NOTE_G, NOTE_G, NOTE_A, NOTE_G, NOTE_C, NOTE_B, NOTE_G, NOTE_G, NOTE_A, NOTE_G, NOTE_D, NOTE_C, NOTE_G, NOTE_G, NOTE_G, NOTE_E, NOTE_C, NOTE_B, NOTE_A, NOTE_F, NOTE_F, NOTE_E, NOTE_C, NOTE_D, NOTE_C, 0, 0, 0},
  {200,    200,    200,    200,    200,    400,    200,    200,    200,    200,    200,    400,    200,    200,    200,    200,    200,    200,    400,    200,    200,    200,    200,    200,    400,    0, 0, 0}
};

song Chizhick = {
  15,
  {NOTE_C, NOTE_A, NOTE_C, NOTE_A, NOTE_D, NOTE_C, NOTE_B, NOTE_0, NOTE_D, NOTE_D, NOTE_D, NOTE_E, NOTE_D, NOTE_B, NOTE_B, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {300,    300,    300,    300,    300,    300,    400,    100,    300,    300,    300,    300,    300,    300,    400,    0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

song SpyatUstalieIgrushki = {
  32,
  {NOTE_E, NOTE_C, NOTE_E, NOTE_C, NOTE_E, NOTE_E, NOTE_D, NOTE_C, NOTE_B, NOTE_C, NOTE_E, NOTE_B, NOTE_C, NOTE_B, NOTE_C, NOTE_E, NOTE_E, NOTE_B, NOTE_E, NOTE_B, NOTE_E, NOTE_D, NOTE_C, NOTE_B, NOTE_A, NOTE_C, NOTE_E, NOTE_B},
  {400,    400,    400,    400,    400,    400,    400,    400,    400,    400,    600,    200,    200,    200,    200,    400,    400,    400,    400,    400,    400,    400,    400,    400,    400,    400,    400,    600}
};


song Elochka = { // ???
  28,
  {NOTE_F, NOTE_C, NOTE_F, NOTE_C, NOTE_F, NOTE_C, NOTE_F, NOTE_F, NOTE_F, NOTE_C, NOTE_F, NOTE_C, NOTE_F, NOTE_F, NOTE_B, NOTE_F, NOTE_C, NOTE_F, NOTE_C, NOTE_F, NOTE_E, NOTE_D, NOTE_C, NOTE_F, NOTE_C, NOTE_F, NOTE_E, NOTE_D},
  {500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    700,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    500,    700}
};

song Gusi = {
  26,
  {NOTE_D, NOTE_C, NOTE_B, NOTE_A, NOTE_E, NOTE_E, NOTE_D, NOTE_C, NOTE_B, NOTE_A, NOTE_E, NOTE_E, NOTE_D, NOTE_F, NOTE_F, NOTE_D, NOTE_C, NOTE_E, NOTE_E, NOTE_C, NOTE_B, NOTE_C, NOTE_D, NOTE_B, NOTE_A, NOTE_A, 0, 0},
  {300,    300,    300,    300,    500,    300,    300,    300,    300,    300,    500,    300,    300,    300,    400,    300,    300,    300,    400,    300,    300,    300,    300,    300,    500,    300,    0, 0}
};

song JingleBells = {
  24,
  {NOTE_E, NOTE_D, NOTE_E, NOTE_E, NOTE_D, NOTE_E, NOTE_E, NOTE_G, NOTE_C, NOTE_D, NOTE_E, NOTE_F, NOTE_F, NOTE_E, NOTE_F, NOTE_F, NOTE_E, NOTE_E, NOTE_D, NOTE_E, NOTE_D, NOTE_D, NOTE_E, NOTE_D, 0, 0, 0, 0},
  {200,    200,    300,    200,    200,    300,    200,    200,    200,    200,    400,    200,    200,    300,    200,    200,    200,    300,    200,    200,    200,    200,    200,    400,    0, 0, 0, 0}
};

song Random = {
  0,
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
  {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};


