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

LedColor::LedColor(const bool needRandomColor) : r(0), g(0), b(0) {
    if (needRandomColor)
        this->RequestNewColor();
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

void LedColor::RequestNewColor() {
    int combination = random(6);
    this->r = combination < 2 ? 0 : (combination < 4 ? 255 : random(256));
    this->g = combination == 2 || combination == 4 ? 0 : (combination == 0 || combination == 5 ? 255 : random(256));
    this->b = combination == 3 || combination == 5 ? 0 : (combination == 1 || combination == 4 ? 255 : random(256));
}

void LedColor::Copy(const LedColor* leds){
    this->r = leds->r;
    this->g = leds->g;
    this->b = leds->b;
};

void LedColor::UpdateColor(const int r, const int g, const int b){
    this->r = r;
    this->g = g;
    this->b = b;
}

/* ##############################################
################## LEDENDPOINT ##################
############################################## */

LedEndpoint::LedEndpoint(const int changeColorTime, const int step)
 : changeColorTime(changeColorTime), step(step), currentStep(0) {
    this->current_color = new LedColor(false);
    this->previous_color = new LedColor(true);
    this->next_color = new LedColor(true);
}

LedEndpoint::~LedEndpoint() {
    delete this->current_color;
    delete this->previous_color;
    delete this->next_color;
}

void LedEndpoint::requestNewColor() {
    this->currentStep = 0;
    this->previous_color->Copy(this->next_color);
    this->next_color->RequestNewColor();
}

LedColor* LedEndpoint::GetCurrentColor() const{
    return this->current_color;
}

void LedEndpoint::ComputeNext(){
    if (this->currentStep == this->changeColorTime){
        this->requestNewColor();
    }

    this->currentStep += this->step;
    if (this->currentStep > this->changeColorTime){
        this->currentStep = this->changeColorTime;
    }

    float factor = (float)this->currentStep / (float)this->changeColorTime;
    this->current_color->UpdateColor(
        this->previous_color->GetR() + (this->next_color->GetR() - this->previous_color->GetR()) * factor,
        this->previous_color->GetG() + (this->next_color->GetG() - this->previous_color->GetG()) * factor,
        this->previous_color->GetB() + (this->next_color->GetB() - this->previous_color->GetB()) * factor
    );
}

/* ##############################################
################# ADAFRUITLEDS ##################
############################################## */

AdafruitLeds::AdafruitLeds(const int leds_pin, const float brightness_percent, const int leds_delay, const int fadingTime, const int changeColorTime)
 : Adafruit_NeoPixel(AdafruitLeds::NUMPIXELS, leds_pin, NEO_GRB + NEO_KHZ800), leds_delay(leds_delay) {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    this->first_endpoint = new LedEndpoint(changeColorTime, leds_delay);
    this->last_endpoint = new LedEndpoint(changeColorTime, leds_delay);
    this->fadingSystem = new FadingSystem(fadingTime, leds_delay);

    this->begin();
    
    this->setBrightness((int)(brightness_percent * 255.0));
    this->clear();
}

AdafruitLeds::~AdafruitLeds() {
    delete this->first_endpoint;
    delete this->last_endpoint;
    delete this->fadingSystem;
}

int AdafruitLeds::GetLedsDelay() const {
    return this->leds_delay;
}

void AdafruitLeds::computeNext() {
    this->fadingSystem->ComputeNext();
    this->first_endpoint->ComputeNext();
    this->last_endpoint->ComputeNext();
}

void AdafruitLeds::ShowNext() {
    this->computeNext();
    float brightFactor = this->fadingSystem->GetBrightness();

    for (int i = 1; i >= -1; i -= 2){
        LedColor* color = i == 1 ? this->first_endpoint->GetCurrentColor() : this->last_endpoint->GetCurrentColor();
        
        for(int j = 0; j < NUMPIXELS / 2; j++){
            int index = (NUMPIXELS - 1 + j * i) % (NUMPIXELS - 1);
            if(index == 0 && j == 0 && i == -1)
                index = NUMPIXELS - 1;

            this->setPixelColor(index,
            index < NUMPIXELS / 4 || index >= NUMPIXELS * 3 / 4 ?
            this->Color(color->GetR() * brightFactor,
                        color->GetG() * brightFactor,
                        color->GetB() * brightFactor) :
            this->Color((int)(((float)color->GetR() + (float)(255 - color->GetR()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * brightFactor),
                        (int)(((float)color->GetG() + (float)(255 - color->GetG()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * brightFactor),
                        (int)(((float)color->GetB() + (float)(255 - color->GetB()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * brightFactor)));
        }
    }

    this->show();
}

void AdafruitLeds::ToggleLight() {
    this->fadingSystem->ToggleFade();
}
