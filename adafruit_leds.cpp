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
################## TOPBEHAVIOR ##################
############################################## */

TopBehavior::TopBehavior(const int step, const int lightTime, const int waitingTime, const int ledCount, const float brightFactor)
 : step(step), lightTime(lightTime), waitingTime(waitingTime), currentStep(0), ledCount(ledCount), brightFactor(brightFactor), isWaiting(true) {

    this->requestNewPosition();
}

void TopBehavior::ComputeNext(){
    if (this->currentStep >= (this->isWaiting ? this->waitingTime : this->lightTime)){
        this->currentStep = 0;
        this->isWaiting = !this->isWaiting;

        if (!this->isWaiting){
            this->requestNewPosition();
        }
    }

    this->currentStep += this->step;
}

void TopBehavior::requestNewPosition(){
    this->position = random(this->ledCount);
}

int TopBehavior::GetPosition() const {
    return this->position;
}

float TopBehavior::GetOriginalBrightFactor() const {
    return this->brightFactor;
}

float TopBehavior::GetCurrentBrightFactor() const {
    if (this->isWaiting)
        return 0.0;
    else{
        float factor = (float)this->currentStep / (float)this->lightTime;
        return (factor <= 0.5 ? factor : 1.0 - factor) * 2 * this->brightFactor;
    }
}

/* ##############################################
################# ADAFRUITLEDS ##################
############################################## */

AdafruitLeds::AdafruitLeds(const int leds_pin, const float brightness_percent, const int leds_delay, const int fadingTime, const int changeColorTime, const int topLightTime, const int topWaitingTime, const float topBrightFactor)
 : Adafruit_NeoPixel(AdafruitLeds::NUMPIXELS, leds_pin, NEO_GRB + NEO_KHZ800), leds_delay(leds_delay) {

#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
    clock_prescale_set(clock_div_1);
#endif

    this->first_endpoint = new LedEndpoint(changeColorTime, leds_delay);
    this->last_endpoint = new LedEndpoint(changeColorTime, leds_delay);
    this->fadingSystem = new FadingSystem(fadingTime, leds_delay);

    this->topBehavior = new TopBehavior(leds_delay, topLightTime, topWaitingTime, AdafruitLeds::NUMPIXELS, topBrightFactor);

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

    this->topBehavior->ComputeNext();
}

void AdafruitLeds::ShowNext() {
    this->computeNext();
    float fadeBrightFactor = this->fadingSystem->GetBrightness();

    float minusBright = this->topBehavior->GetOriginalBrightFactor();
    float topBrightFactor = this->topBehavior->GetCurrentBrightFactor();
    int topPosition = this->topBehavior->GetPosition();

    for (int i = 1; i >= -1; i -= 2){
        LedColor* color = i == 1 ? this->first_endpoint->GetCurrentColor() : this->last_endpoint->GetCurrentColor();
        
        for(int j = 0; j < NUMPIXELS / 2; j++){
            int index = (NUMPIXELS - 1 + j * i) % (NUMPIXELS - 1);
            if(index == 0 && j == 0 && i == -1)
                index = NUMPIXELS - 1;

            float totalBright = fadeBrightFactor * (1.0 - minusBright + (topPosition == index ? topBrightFactor : 0));

            this->setPixelColor(index,
            index < NUMPIXELS / 4 || index >= NUMPIXELS * 3 / 4 ?
            this->Color(color->GetR() * totalBright,
                        color->GetG() * totalBright,
                        color->GetB() * totalBright) :
            this->Color((int)(((float)color->GetR() + (float)(255 - color->GetR()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * totalBright),
                        (int)(((float)color->GetG() + (float)(255 - color->GetG()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * totalBright),
                        (int)(((float)color->GetB() + (float)(255 - color->GetB()) / (float)(NUMPIXELS / 4 - 1) * (float)(j - NUMPIXELS / 4)) * totalBright)));
        }
    }

    this->show();
}

void AdafruitLeds::ToggleLight() {
    this->fadingSystem->ToggleFade();
}
