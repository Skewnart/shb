#include "clock.h"
#include "ultrasonic_sensor.h"
#include "button.h"

#define DELAY_INTERVAL 100

Clock* clock;
UltrasonicSensor* ultrasonic_sensor; 
Button* button;
bool buttonWaitForRelease;

void setup() {
  Serial.begin(9600);

  clock = new Clock(DELAY_INTERVAL);
  ultrasonic_sensor = new UltrasonicSensor();
  button = new Button(6, DELAY_INTERVAL);
  buttonWaitForRelease = false;
}

void loop() {  
  ButtonState* state = button->UpdateState(100);
  int distance = ultrasonic_sensor->GetDistanceMm();

  if (buttonWaitForRelease && state->current == RELEASED)
    buttonWaitForRelease = false;

  if (state->current == PRESSED && state->since >= 3000 && !buttonWaitForRelease){
    if (clock->GetMode() == SETTINGS)
      clock->SettingsNextStep();
    else
      clock->SetSettingsMode();
      
    buttonWaitForRelease = true;
  }

  if (clock->GetMode() == SETTINGS) {
    if (distance > 100){
      clock->SettingsChangeTime(true);
    }
    else if (distance < 5){
      clock->SettingsChangeTime(false);
    }
  }
  else {
    if (distance >= 10 && distance < 50){
      clock->ShowDuringSeconds(3);
    }
  }

  clock->UpdateDisplay();
  delay(DELAY_INTERVAL);
}
