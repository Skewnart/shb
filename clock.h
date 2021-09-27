#ifndef clock_h
#define clock_h

#include <TM1637Display.h>

class Clock : TM1637Display {

    private :
        bool h12, PM;

    public :
        const int CLK = 2;
        const int DIO = 3;

        Clock();

        void Display(int);
};

#endif
