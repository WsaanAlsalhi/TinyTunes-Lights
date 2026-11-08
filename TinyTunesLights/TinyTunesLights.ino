/*
 * TinyTunes Lights
 * ----------------
 * Board : STM32F103C8T6 (Blue Pill)
 * Parts : 6 push buttons, 6 LEDs, 1 passive buzzer
 *
 * Each button plays its own kids' melody on the buzzer.
 * The LED pattern is driven by the melody itself (note pitch),
 * every song has a different pattern style, and every pattern
 * starts from the LED that belongs to the pressed button.
 *
 * Non-blocking: no delay() is used, so pressing another button
 * instantly interrupts the current song and starts the new one.
 */

// ---------------------------------------------------------------
// Pin configuration
// ---------------------------------------------------------------
const int ledPins[6] = {PA0, PA1, PA2, PA3, PB0, PB1};      // LED 1..6 (220-330 ohm resistor to GND)
const int btnPins[6] = {PB12, PB13, PB14, PB15, PA6, PA7};  // Button 1..6 (other side to GND)
const int buzzerPin  = PA8;                                  // Passive buzzer (other side to GND)

// ---------------------------------------------------------------
// Note frequencies (Hz)
// ---------------------------------------------------------------
#define NOTE_C4 262
#define NOTE_D4 294
#define NOTE_E4 330
#define NOTE_F4 349
#define NOTE_G4 392
#define NOTE_A4 440
#define NOTE_B4 494
#define NOTE_C5 523
#define NOTE_D5 587
#define NOTE_E5 659
#define NOTE_F5 698
#define NOTE_G5 784

// Duration values: 1 = whole, 2 = half, 4 = quarter, 8 = eighth
// Note length (ms) = song tempo / duration value

// ---------------------------------------------------------------
// Song 1: Twinkle Twinkle Little Star
// ---------------------------------------------------------------
const int s1n[] = {
  NOTE_C4,NOTE_C4,NOTE_G4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_G4,
  NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,NOTE_C4,
  NOTE_G4,NOTE_G4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,
  NOTE_G4,NOTE_G4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,
  NOTE_C4,NOTE_C4,NOTE_G4,NOTE_G4,NOTE_A4,NOTE_A4,NOTE_G4,
  NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,NOTE_C4
};
const int s1d[] = {
  4,4,4,4,4,4,2, 4,4,4,4,4,4,2, 4,4,4,4,4,4,2,
  4,4,4,4,4,4,2, 4,4,4,4,4,4,2, 4,4,4,4,4,4,2
};

// ---------------------------------------------------------------
// Song 2: Mary Had a Little Lamb
// ---------------------------------------------------------------
const int s2n[] = {
  NOTE_E4,NOTE_D4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_D4,NOTE_D4,NOTE_D4, NOTE_E4,NOTE_G4,NOTE_G4,
  NOTE_E4,NOTE_D4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_D4,NOTE_D4,NOTE_E4,NOTE_D4,NOTE_C4
};
const int s2d[] = {
  4,4,4,4,4,4,2, 4,4,2, 4,4,2,
  4,4,4,4,4,4,4,4, 4,4,4,4,1
};

// ---------------------------------------------------------------
// Song 3: Jingle Bells
// ---------------------------------------------------------------
const int s3n[] = {
  NOTE_E4,NOTE_E4,NOTE_E4, NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_G4,NOTE_C4,NOTE_D4,NOTE_E4,
  NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_F4,NOTE_E4,NOTE_E4,NOTE_E4,NOTE_E4,
  NOTE_E4,NOTE_D4,NOTE_D4,NOTE_E4,NOTE_D4,NOTE_G4
};
const int s3d[] = {
  4,4,2, 4,4,2, 4,4,4,4,1,
  4,4,4,4,4,4,4,8,8, 4,4,4,4,2,2
};

// ---------------------------------------------------------------
// Song 4: Happy Birthday
// ---------------------------------------------------------------
const int s4n[] = {
  NOTE_G4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_C5,NOTE_B4,
  NOTE_G4,NOTE_G4,NOTE_A4,NOTE_G4,NOTE_D5,NOTE_C5,
  NOTE_G4,NOTE_G4,NOTE_G5,NOTE_E5,NOTE_C5,NOTE_B4,NOTE_A4,
  NOTE_F5,NOTE_F5,NOTE_E5,NOTE_C5,NOTE_D5,NOTE_C5
};
const int s4d[] = {
  8,8,4,4,4,2, 8,8,4,4,4,2, 8,8,4,4,4,4,2, 8,8,4,4,4,2
};

// ---------------------------------------------------------------
// Song 5: Ode to Joy
// ---------------------------------------------------------------
const int s5n[] = {
  NOTE_E4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,
  NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_E4,NOTE_D4,NOTE_D4,
  NOTE_E4,NOTE_E4,NOTE_F4,NOTE_G4,NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,
  NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,NOTE_D4,NOTE_C4,NOTE_C4
};
const int s5d[] = {
  4,4,4,4,4,4,4,4, 4,4,4,4,3,8,2,
  4,4,4,4,4,4,4,4, 4,4,4,4,3,8,2
};

// ---------------------------------------------------------------
// Song 6: Row, Row, Row Your Boat
// ---------------------------------------------------------------
const int s6n[] = {
  NOTE_C4,NOTE_C4,NOTE_C4,NOTE_D4,NOTE_E4,
  NOTE_E4,NOTE_D4,NOTE_E4,NOTE_F4,NOTE_G4,
  NOTE_C5,NOTE_C5,NOTE_C5,NOTE_G4,NOTE_G4,NOTE_G4,
  NOTE_E4,NOTE_E4,NOTE_E4,NOTE_C4,NOTE_C4,NOTE_C4,
  NOTE_G4,NOTE_F4,NOTE_E4,NOTE_D4,NOTE_C4
};
const int s6d[] = {
  4,4,4,8,2, 4,8,4,8,2,
  8,8,8,8,8,8,8,8,8,8,8,8,
  4,8,4,8,2
};

// ---------------------------------------------------------------
// LED pattern styles (one per song)
// ---------------------------------------------------------------
enum PatternMode {
  PITCH_FOLLOW,  // single LED follows the pitch up and down
  MIRROR,        // two LEDs move in opposite directions
  BAR_METER,     // higher note = more LEDs lit
  EXPAND,        // light spreads from the start LED to both sides
  BOUNCE,        // light bounces back and forth, pitch changes the jump
  TRAIL          // head LED with a 3-LED tail following the melody direction
};

struct Song {
  const int* notes;   // note frequencies
  const int* durs;    // note duration values
  int length;         // number of notes
  int tempo;          // whole-note length in ms (lower = faster)
  PatternMode mode;   // LED pattern style
};

const Song songs[6] = {
  { s1n, s1d, sizeof(s1n)/sizeof(int), 2000, PITCH_FOLLOW },
  { s2n, s2d, sizeof(s2n)/sizeof(int), 1800, MIRROR      },
  { s3n, s3d, sizeof(s3n)/sizeof(int), 1600, BAR_METER   },
  { s4n, s4d, sizeof(s4n)/sizeof(int), 2000, EXPAND      },
  { s5n, s5d, sizeof(s5n)/sizeof(int), 1800, BOUNCE      },
  { s6n, s6d, sizeof(s6n)/sizeof(int), 1800, TRAIL       },
};

// Scale used to convert a frequency into a note degree (0..11)
const int scaleFreq[] = {262,294,330,349,392,440,494,523,587,659,698,784};
const int SCALE_N = sizeof(scaleFreq)/sizeof(int);

int degreeOf(int freq) {
  for (int i = 0; i < SCALE_N; i++) if (scaleFreq[i] == freq) return i;
  return 0;
}

// ---------------------------------------------------------------
// Playback state
// ---------------------------------------------------------------
bool playing = false;
int  curSong = 0;            // index of the current song (= button index)
int  noteIdx = 0;            // index of the current note
unsigned long noteStart = 0; // when the current note started (ms)
unsigned long noteLen   = 0; // length of the current note (ms)
bool toneOn = false;         // true while the buzzer is sounding

bool lastBtn[6] = {true, true, true, true, true, true};  // previous button states (HIGH = released)
unsigned long lastPress[6] = {0};                        // last press time, for debounce

// ---------------------------------------------------------------
// LED helpers
// ---------------------------------------------------------------
void allLedsOff() {
  for (int i = 0; i < 6; i++) digitalWrite(ledPins[i], LOW);
}

// Wrap any index into 0..5 (works for negative numbers too)
inline int wrap6(int x) { return ((x % 6) + 6) % 6; }

// Turn on one LED using a wrapped index
inline void ledOn(int i) { digitalWrite(ledPins[wrap6(i)], HIGH); }

// ---------------------------------------------------------------
// Pattern engine: draws the LED pattern for the current note.
// "start" is the LED of the pressed button, and "rel" is how far
// the current note is from the first note of the song.
// ---------------------------------------------------------------
void showPattern() {
  const Song& s = songs[curSong];
  int start = curSong;
  int rel   = degreeOf(s.notes[noteIdx]) - degreeOf(s.notes[0]);
  allLedsOff();

  switch (s.mode) {

    case PITCH_FOLLOW:
      ledOn(start + rel);
      break;

    case MIRROR:
      ledOn(start + rel);
      ledOn(start - rel);
      break;

    case BAR_METER: {
      int count = wrap6(rel) + 1;
      for (int k = 0; k < count; k++) ledOn(start + k);
      break;
    }

    case EXPAND: {
      int w = wrap6(rel) % 4;   // spread width 0..3
      ledOn(start);
      for (int k = 1; k <= w; k++) { ledOn(start + k); ledOn(start - k); }
      break;
    }

    case BOUNCE: {
      int p = (noteIdx + abs(rel)) % 10;
      if (p > 5) p = 10 - p;    // bounce back after reaching the end
      ledOn(start + p);
      break;
    }

    case TRAIL: {
      int head = start + rel;
      int prev = (noteIdx > 0) ? noteIdx - 1 : 0;
      int dir  = (s.notes[noteIdx] >= s.notes[prev]) ? 1 : -1;  // melody going up or down
      ledOn(head);
      ledOn(head - dir);
      ledOn(head - 2 * dir);
      break;
    }
  }
}

// ---------------------------------------------------------------
// Song control
// ---------------------------------------------------------------
void startNote() {
  const Song& s = songs[curSong];
  noteLen   = s.tempo / s.durs[noteIdx];
  noteStart = millis();
  showPattern();
  tone(buzzerPin, s.notes[noteIdx]);
  toneOn = true;
}

void startSong(int idx) {
  noTone(buzzerPin);   // cut whatever is playing
  curSong = idx;
  noteIdx = 0;
  playing = true;
  startNote();         // first note lights the pressed button's LED
}

void stopSong() {
  noTone(buzzerPin);
  allLedsOff();
  playing = false;
  toneOn  = false;
}

// ---------------------------------------------------------------
// Arduino entry points
// ---------------------------------------------------------------
void setup() {
  for (int i = 0; i < 6; i++) {
    pinMode(ledPins[i], OUTPUT);
    pinMode(btnPins[i], INPUT_PULLUP);   // internal pull-up, button connects pin to GND
  }
  pinMode(buzzerPin, OUTPUT);
  allLedsOff();
}

void loop() {
  unsigned long now = millis();

  // Read buttons (falling edge + 50 ms debounce)
  for (int i = 0; i < 6; i++) {
    bool state = digitalRead(btnPins[i]);
    if (lastBtn[i] == HIGH && state == LOW && now - lastPress[i] > 50) {
      lastPress[i] = now;
      startSong(i);   // any button interrupts the current song
    }
    lastBtn[i] = state;
  }

  // Non-blocking melody player
  if (playing) {
    // Short silence at the end of each note so repeated notes are distinct
    if (toneOn && now - noteStart >= noteLen * 0.9) {
      noTone(buzzerPin);
      toneOn = false;
    }
    // Move to the next note
    if (now - noteStart >= noteLen) {
      noteIdx++;
      if (noteIdx >= songs[curSong].length) stopSong();   // use "noteIdx = 0; startNote();" to loop
      else startNote();
    }
  }
}
