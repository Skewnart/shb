#include "clock.h"

Clock::Clock(const int& interval) : TM1637Display(Clock::CLK, Clock::DIO) {
    this->clear();
    this->setBrightness(7);
    this->displayMsRemaining = 0;
    this->update_interval = interval;

    this->rtc = new DS3231();
}

Clock::~Clock() {
    delete this->rtc;
}

void Clock::UpdateDisplay() {
    if (this->displayMsRemaining > 0)
    {
        this->showNumberDecEx(this->rtc->getHour(h12, PM) * 100 + this->rtc->getMinute(),
            0b11100000, false, 4, 0);
            
        this->displayMsRemaining -=
            (this->displayMsRemaining > this->update_interval ? this->update_interval : this->displayMsRemaining);

        if (this->displayMsRemaining == 0){
            this->clear();
        }
    }
}

void Clock::SetTime(const int& hour, const int& minute){
    this->rtc->setHour(hour);
    this->rtc->setMinute(minute);  
}

void Clock::ShowDuringSeconds(const int& seconds){
    this->displayMsRemaining = seconds * 1000;
}
