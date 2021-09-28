#include "ultrasonic_sensor.h"

UltrasonicSensor::UltrasonicSensor() {
    pinMode(UltrasonicSensor::TRIGGER_PIN, OUTPUT);
    digitalWrite(UltrasonicSensor::TRIGGER_PIN, LOW);
    pinMode(UltrasonicSensor::ECHO_PIN, INPUT);
}

int UltrasonicSensor::GetDistanceMm(void) const {
    digitalWrite(UltrasonicSensor::TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(UltrasonicSensor::TRIGGER_PIN, LOW);
  
    long measure = pulseIn(UltrasonicSensor::ECHO_PIN, HIGH, UltrasonicSensor::MEASURE_TIMEOUT);
    return int(measure / 2.0 * UltrasonicSensor::SOUND_SPEED);
}