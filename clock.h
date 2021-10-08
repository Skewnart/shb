#ifndef clock_h
#define clock_h

#include <TM1637Display.h>
#include <DS3231.h>

class Clock : TM1637Display {

    private :
        DS3231* rtc;
        bool h12, PM;
        int displayMsRemaining;
        int update_interval;

        static const int CLK = 2;
        static const int DIO = 3;

    public :
        Clock(const int&);
        ~Clock();

        void UpdateDisplay();
        void SetTime(const int&, const int&);
        void ShowDuringSeconds(const int&);
};

#endif
