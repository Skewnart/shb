#include "clock.h"

Clock::Clock() : TM1637Display(CLK, DIO){
    this->clear();
    this->setBrightness(7);
}

void Clock::Display(int number) {
    this->showNumberDec(number);
}
