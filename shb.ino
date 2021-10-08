#include "clock.h"
#include "ultrasonic_sensor.h"
#include "button.h"

#define DELAY_INTERVAL 100

Clock* clock;
UltrasonicSensor* ultrasonic_sensor; 
Button* button;

void setup() {
  Serial.begin(9600);

  clock = new Clock(DELAY_INTERVAL);
  ultrasonic_sensor = new UltrasonicSensor();
  button = new Button(6, DELAY_INTERVAL);
}

void loop() {
  //clock->DisplayTime();
  //Serial.println(ultrasonic_sensor->GetDistanceMm());
  
  // ButtonState* state = button->UpdateState(100);
  // if ((int)state->current == RELEASED && (int)state->previous == PRESSED){
  //   Serial.println("Bouton relâché !");
  // }
  // else if ((int)state->current == PRESSED){
  //   Serial.print("Bouton appuyé depuis ");
  //   Serial.print(state->since / 1000.0);
  //   Serial.println(" secondes !");
  // }

  int distance = ultrasonic_sensor->GetDistanceMm();
  if (distance >= 10 && distance < 50){
    clock->ShowDuringSeconds(3);
  }

  clock->UpdateDisplay();
  delay(DELAY_INTERVAL);
}
