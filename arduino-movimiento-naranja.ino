#include "pitches.h"

const int melodies[2][8] = {
  { NOTE_E3, NOTE_G3, NOTE_B3, NOTE_G3, NOTE_E3, NOTE_C4, NOTE_B3, NOTE_C4 },
  { NOTE_G3, NOTE_B3, NOTE_D4, NOTE_B3, NOTE_G3, NOTE_A3, NOTE_G3, NOTE_A3 }
};

const int noteDurations[] = {
  4, 8, 4, 8, 2, 4, 4, 4
};

const int tempo = 800;

const int ldrPin = A0;
const int buzzerPin = 3;
const int avgLightBuckets = 32;
const int percentageDropThreshold = -15;

int currentLightBucket = 0; // round robin pointer
int lightLevels[avgLightBuckets];

int songSegment = 1;


void playMelody(int segment) {
  for (int thisNote = 0; thisNote < 8; thisNote++) {
    int noteDuration = tempo / noteDurations[thisNote];
    tone(buzzerPin, melodies[segment][thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);

    noTone(buzzerPin);
  }
}

void updateLight(int lightLevel) {
  lightLevels[currentLightBucket] = lightLevel;
  currentLightBucket = (currentLightBucket + 1) % avgLightBuckets;
}

int lightDropDetected(int lightLevel) {
  int avgLightLevel = 0;
  
  for (int i = 0; i < avgLightBuckets; i++) {
    avgLightLevel = avgLightLevel + lightLevels[i];
  }
  avgLightLevel = avgLightLevel / avgLightBuckets;
  if (avgLightLevel == 0) return 0; // to avoid division by 0
  
  int deltaLightLevel = lightLevel - avgLightLevel;
  int percentageDrop = (deltaLightLevel * 100) / avgLightLevel;

  if (percentageDrop < percentageDropThreshold) return 1;
  
  return 0;
}

void setup() {  
  for (int i = 0; i < avgLightBuckets; i++ ) {
    updateLight(analogRead(ldrPin));
    delay(10);
  }
}

void loop() {
  int lightLevel = analogRead(ldrPin);
  updateLight(lightLevel);

  if (lightDropDetected(lightLevel)) {
    playMelody(songSegment = !songSegment);
  }
  delay(100);
}

