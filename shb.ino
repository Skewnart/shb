#include "clock.h"
#include "ultrasonic_sensor.h"
#include "button.h"

#define DELAY_INTERVAL 100

Clock* clock;
UltrasonicSensor* ultrasonic_sensor;
Button *button1, *button2;
bool buttonWaitForRelease;

void setup() {
  Serial.begin(9600);

  clock = new Clock(DELAY_INTERVAL);
  ultrasonic_sensor = new UltrasonicSensor();
  button1 = new Button(6, DELAY_INTERVAL);
  button2 = new Button(7, DELAY_INTERVAL);
  buttonWaitForRelease = false;
}

void loop() {
  ButtonState *state_btn1 = button1->UpdateState(DELAY_INTERVAL),
               *state_btn2 = button2->UpdateState(DELAY_INTERVAL);
  int distance = ultrasonic_sensor->GetDistanceMm();

  if (((state_btn1->current == PRESSED && state_btn1->since >= 3000) || (state_btn2->current == PRESSED && state_btn2->since >= 3000))
      && !buttonWaitForRelease) {
    if (clock->GetMode() == SETTINGS)
      clock->SettingsNextStep();
    else {
      clock->SetSettingsMode();
    }

    buttonWaitForRelease = true;
  }

  if (state_btn1->current == RELEASED && state_btn2->current == RELEASED) {
    if (buttonWaitForRelease) {
      buttonWaitForRelease = false;
    }
    else if ((state_btn1->previous == PRESSED || state_btn2->previous == PRESSED)
             && clock->GetMode() == SETTINGS) {
      clock->ToggleSettingsLocked();
    }
  }

  if (clock->GetMode() == SETTINGS) {
    if (!clock->GetSettingsLocked()) {
      if (distance > 100) {
        clock->SettingsChangeTime(true);
      }
      else if (distance < 50) {
        clock->SettingsChangeTime(false);
      }
    }
  }
  else {
    if (distance >= 10 && distance < 50) {
      clock->ShowDuringSeconds(3);
    }
  }

  clock->UpdateDisplay();
  delay(DELAY_INTERVAL);
}
