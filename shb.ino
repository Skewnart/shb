#include "clock.h"
#include "ultrasonic_sensor.h"
#include "button.h"
#include "adafruit_leds.h"

#define DELAY_INTERVAL 100
#define PIN_FOR_RANDOM A0

Clock* clock;
UltrasonicSensor* ultrasonic_sensor;
Button *button1, *button2;
AdafruitLeds *leds;
bool buttonWaitForRelease;

void setup() {
  Serial.begin(9600);
  randomSeed(analogRead(PIN_FOR_RANDOM));

  clock = new Clock(DELAY_INTERVAL);
  ultrasonic_sensor = new UltrasonicSensor();
  button1 = new Button(6, DELAY_INTERVAL);
  button2 = new Button(7, DELAY_INTERVAL);
  leds = new AdafruitLeds(8, 1.0, 10, 1000, 2000, 2000, 750, 0.2);
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
    else if (state_btn1->previous == PRESSED || state_btn2->previous == PRESSED) {
      if (clock->GetMode() == SETTINGS) {
        clock->ToggleSettingsLocked();
      }
      else {
        leds->ToggleLight();
      }
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

  for (int i = 0; i < DELAY_INTERVAL; i += leds->GetLedsDelay()) {
    leds->ShowNext();
    delay(leds->GetLedsDelay());
  }
}
