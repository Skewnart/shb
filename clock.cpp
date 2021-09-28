#include "clock.h"

Clock::Clock() : TM1637Display(Clock::CLK, Clock::DIO) {
    this->clear();
    this->setBrightness(7);

    this->rtc = new DS3231();
}

Clock::~Clock() {
    delete this->rtc;
}

void Clock::DisplayTime() {
    this->showNumberDecEx(this->rtc->getHour(h12, PM) * 100 + this->rtc->getMinute(),
        0b11100000, false, 4, 0);
}

void Clock::SetTime(int hour, int minute){
    this->rtc->setHour(hour);
    this->rtc->setMinute(minute);  
}
