#include "clock.h"
#include "ultrasonic_sensor.h"
#include "button.h"

Clock* clock;
UltrasonicSensor* ultrasonic_sensor; 
Button* button;

void setup() {
  Serial.begin(9600);

  clock = new Clock();
  ultrasonic_sensor = new UltrasonicSensor();
  button = new Button(6);
}

void loop() {
  //clock->DisplayTime();
  //Serial.println(ultrasonic_sensor->GetDistanceMm());
  
  ButtonState* state = button->UpdateState(100);
  Serial.print((int)state->state == 0 ? "Appuyé" : "Non appuyé");
  Serial.print(" depuis ");
  Serial.print(state->since / 1000.0);
  Serial.println(" secondes");
  
  delay(100);
}
