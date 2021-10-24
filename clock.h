#ifndef clock_h
#define clock_h

#include <TM1637Display.h>
#include <DS3231.h>

enum ClockMode{
    HOUR,
    SETTINGS
};

class Clock : TM1637Display {

    private :
        DS3231* rtc;
        bool h12, PM;
        int displayMsRemaining;
        int update_interval;
        ClockMode mode;
        int settingsStep;

        static const int CLK = 2;
        static const int DIO = 3;
        static constexpr unsigned char SETTINGSSEG[] = {
            SEG_A | SEG_D | SEG_G,
            SEG_A | SEG_D | SEG_G
        };

    public :
        Clock(const int&);
        ~Clock();

        void UpdateDisplay();
        void SetTime(const int&, const int&);
        void ShowDuringSeconds(const int&);
        ClockMode GetMode() const;
        void SetSettingsMode();
        void SettingsNextStep();
        void SettingsChangeTime(const bool);
};

#endif
