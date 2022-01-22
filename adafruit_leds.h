#ifndef adafruit_leds_h
#define adafruit_leds_h

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

class FadingSystem{
    private :
        bool increasing;
        int fadingTime, step, currentStep;

    public :
        FadingSystem(const int, const int);
        void ToggleFade();
        void ComputeNext();
        float GetBrightness() const;
};

class LedColor{
    private :
        uint8_t r, g, b;

    public :
        LedColor(const bool);
        
        uint8_t GetR() const;
        uint8_t GetG() const;
        uint8_t GetB() const;

        void RequestNewColor();
        void Copy(const LedColor*);
        void UpdateColor(const int, const int, const int);
};

class LedEndpoint{
    private :
        LedColor *current_color, *previous_color, *next_color;
        int changeColorTime, step, currentStep;
    
        void requestNewColor();

    public :
        LedEndpoint(const int, const int);
        ~LedEndpoint();

        LedColor* GetCurrentColor() const;
        void ComputeNext();
};

class TopBehavior {
    private :
        int step, lightTime, waitingTime, currentStep;
        int position, ledCount;
        float brightFactor;
        bool isWaiting;

        void requestNewPosition();

    public :
        TopBehavior(const int, const int, const int, const int, const float);
        void ComputeNext();

        int GetPosition() const;
        float GetOriginalBrightFactor() const;
        float GetCurrentBrightFactor() const;
};

class AdafruitLeds : Adafruit_NeoPixel{
    
    private :
        static const int NUMPIXELS = 60;

        LedEndpoint *first_endpoint, *last_endpoint;
        int leds_delay;
        FadingSystem* fadingSystem;
        TopBehavior* topBehavior;

        void computeNext();
    
    public :
        AdafruitLeds(const int, const float, const int, const int, const int, const int, const int, const float);
        ~AdafruitLeds();

        int GetLedsDelay() const;
        void ShowNext();
        void ToggleLight();
};

#endif
