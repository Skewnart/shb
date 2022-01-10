#include "adafruit_leds.h"

const int AdafruitLeds::NUMPIXELS;

/* ##############################################
################## FADINGSYSTEM #################
############################################## */

FadingSystem::FadingSystem(const int fadingTime, const int step)
 : fadingTime(fadingTime), increasing(false), step(step), currentStep(0) {

}

void FadingSystem::ComputeNext(){
    if (this->increasing && this->currentStep < this->fadingTime){
        this->currentStep += this->step;
        if (this->currentStep > this->fadingTime){
            this->currentStep = this->fadingTime;
        }
    }
    else if (!this->increasing && this->currentStep > 0){
        this->currentStep -= this->step;
        if (this->currentStep < 0){
            this->currentStep = 0;
        }
    }
}

void FadingSystem::ToggleFade(){
    this->increasing = !this->increasing;
}

float FadingSystem::GetBrightness() const{
    return (float)this->currentStep / (float)this->fadingTime;
}

/* ##############################################
#################### LEDCOLOR ###################
############################################## */

LedColor::LedColor(const uint8_t r, const uint8_t g, const uint8_t b) : r(r), g(g), b(b) {}

void LedEndpoint::RequestNewColor() {

}

uint8_t LedColor::GetR() const{
    return this->r;
}

uint8_t LedColor::GetG() const{
    return this->g;
}

uint8_t LedColor::GetB() const{
    return this->b;
}

/* ##############################################
################## LEDENDPOINT ##################
############################################## */

LedEndpoint::LedEndpoint(const uint8_t r, const uint8_t g, const uint8_t b) {
    this->current_color = new LedColor(r, g, b);
}

LedEndpoint::~LedEndpoint() {
    delete this->current_color;
    delete this->previous_color;
}

LedColor* LedEndpoint::GetCurrentColor() const{
    return this->current_color;
}

LedColor* LedEndpoint::GetPreviousColor() const{
    return this->previous_color;
}

/* ##############################################
################# ADAFRUITLEDS ##################
############################################## */

AdafruitLeds::AdafruitLeds(const int leds_pin, const float brightness_percent, const int leds_delay, const int fadingTime)
 : Adafruit_NeoPixel(AdafruitLeds::NUMPIXELS, leds_pin, NEO_GRB + NEO_KHZ800), leds_delay(leds_delay) {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    this->first_endpoint = new LedEndpoint(255, 255, 255);
    this->last_endpoint = new LedEndpoint(255, 255, 255);
    this->fadingSystem = new FadingSystem(fadingTime, leds_delay);

    this->begin();
    
    this->setBrightness((int)(brightness_percent * 255.0));
    this->clear();
}

AdafruitLeds::~AdafruitLeds() {
    delete this->first_endpoint;
    delete this->last_endpoint;
}

int AdafruitLeds::GetLedsDelay() const {
    return this->leds_delay;
}

void AdafruitLeds::computeNext() {
    this->fadingSystem->ComputeNext();
}

void AdafruitLeds::ShowNext() {
    this->computeNext();
    float brightFactor = this->fadingSystem->GetBrightness();

    for(int i = 0; i < NUMPIXELS; i++) {
        this->setPixelColor(i,
        this->Color(this->first_endpoint->GetCurrentColor()->GetR() * brightFactor,
                    this->first_endpoint->GetCurrentColor()->GetG() * brightFactor,
                    this->first_endpoint->GetCurrentColor()->GetB() * brightFactor));
    }

    this->show();
}

void AdafruitLeds::ToggleLight() {
    this->fadingSystem->ToggleFade();
}
