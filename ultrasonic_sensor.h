#ifndef ultrasonic_sensor_h
#define ultrasonic_sensor_h

#include <Arduino.h>

class UltrasonicSensor {
    
    private :
        static const byte TRIGGER_PIN = 4;
        static const byte ECHO_PIN = 5;
        static const unsigned long MEASURE_TIMEOUT = 25000UL; // 25ms = ~8m à 340m/s
        static constexpr float SOUND_SPEED = 340.0 / 1000;
    
    public :
        UltrasonicSensor();
        int GetDistanceMm(void) const;
};

#endif
