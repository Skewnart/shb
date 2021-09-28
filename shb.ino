#include "clock.h"
#include "ultrasonic_sensor.h"

Clock* clock;
UltrasonicSensor* ultrasonic_sensor;
int num = 0;

void setup() {
  Serial.begin(9600);

  clock = new Clock();
  ultrasonic_sensor = new UltrasonicSensor();
}

void loop() {
  clock->DisplayTime();
  Serial.println(ultrasonic_sensor->GetDistanceMm());
  
  delay(250);
}
