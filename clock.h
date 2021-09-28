#ifndef clock_h
#define clock_h

#include <TM1637Display.h>
#include <DS3231.h>

class Clock : TM1637Display {

    private :
        DS3231* rtc;
        bool h12, PM;

        static const int CLK = 2;
        static const int DIO = 3;

    public :

        Clock();
        ~Clock();

        void DisplayTime();
        void SetTime(int, int);
};

#endif
