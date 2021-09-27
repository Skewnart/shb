#include "clock.h"

Clock* clock;
int num = 0;

void setup() {
  //Set le moniteur série à 9600 bauds
  Serial.begin(9600);

  //Instanciation de la custom clock
  clock = new Clock();
  delay(500);
}

void loop() {
  //Utilisation de la fonction Display
  clock->Display((num = (num + 1) % 10000));
  delay(50);
}
