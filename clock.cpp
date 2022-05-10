#include "clock.h"

const int Clock::CLK;
const int Clock::DIO;
constexpr unsigned char Clock::SETTINGSSEG[];

Clock::Clock(const int& interval) : TM1637Display(Clock::CLK, Clock::DIO) {
    this->clear();
    this->setBrightness(1);
    this->displayMsRemaining = 0;
    this->update_interval = interval;
    this->mode = HOUR;

    this->rtc = new DS3231();
}

Clock::~Clock() {
    delete this->rtc;
}

void Clock::UpdateDisplay() {
    if (this->mode == HOUR){
        if (this->displayMsRemaining > 0)
        {
            this->showNumberDecEx(this->rtc->getHour(h12, PM) * 100 + this->rtc->getMinute(),
                0b11100000, true, 4, 0);
                
            this->displayMsRemaining -=
                (this->displayMsRemaining > this->update_interval ? this->update_interval : this->displayMsRemaining);

            if (this->displayMsRemaining == 0){
                this->clear();
            }
        }
    }
    else{
        if(this->settingsStep == 1){
            this->showNumberDecEx(this->rtc->getHour(h12, PM), 0b11100000, false, 2, 0);
            this->setSegments(Clock::SETTINGSSEG, 2, 2);
        }
        else if(this->settingsStep == 2){
            this->showNumberDecEx(this->rtc->getMinute(), 0b11100000, true, 2, 2);
            this->setSegments(Clock::SETTINGSSEG, 2, 0);
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

void Clock::SetSettingsMode(){
    this->mode = SETTINGS;
    this->displayMsRemaining = 0;
    this->settingsStep = 1;
    this->settingsLocked = true;
}

void Clock::SettingsNextStep(){
    if (++this->settingsStep > 2){
        this->mode = HOUR;
        this->clear();
    }
    this->settingsLocked = true;
}

void Clock::SettingsChangeTime(const bool isIncrease) {
    if (!this->settingsLocked){
        if (this->settingsStep == 1)
            this->SetTime(((this->rtc->getHour(h12, PM) + 24) + (isIncrease ? 1 : -1)) % 24, this->rtc->getMinute());
        else
            this->SetTime(this->rtc->getHour(h12, PM), ((this->rtc->getMinute() + 60) + (isIncrease ? 1 : -1)) % 60);
    }
}

void Clock::ToggleSettingsLocked(){
    this->settingsLocked = !this->settingsLocked;
}

bool Clock::GetSettingsLocked() const {
  return this->settingsLocked;
}

ClockMode Clock::GetMode() const{
    return this->mode;
}
