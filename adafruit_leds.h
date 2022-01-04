#ifndef adafruit_leds_h
#define adafruit_leds_h

#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

class LedColor{
    private :
        uint8_t r, g, b;

    public :
        LedColor(const uint8_t, const uint8_t, const uint8_t);
        
        uint8_t GetR() const;
        uint8_t GetG() const;
        uint8_t GetB() const;
};

class LedEndpoint{
    private :
        LedColor *current_color, *previous_color;

    public :
        LedEndpoint(const uint8_t, const uint8_t, const uint8_t);
        ~LedEndpoint();

        LedColor* GetCurrentColor() const;
        LedColor* GetPreviousColor() const;

        void RequestNewColor();
};

class AdafruitLeds : Adafruit_NeoPixel{
    
    private :
        static const int NUMPIXELS = 60;

        LedEndpoint *first_endpoint, *last_endpoint;
        int leds_delay;
    
    public :
        AdafruitLeds(const int, const float, const int);
        ~AdafruitLeds();

        int GetLedsDelay() const;
        void ShowNext();
};

#endif
