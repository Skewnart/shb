#include "clock.h"

Clock* clock;
int num = 0;

void setup() {
  Serial.begin(9600);

  clock = new Clock();
  clock->SetTime(15, 00);
}

void loop() {
  clock->DisplayTime();
  delay(50);
}
