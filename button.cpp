#include "button.h"

ButtonState::ButtonState()
    : state(RELEASED), since(0){
}

ButtonState* ButtonState::Update(const int interval, const int input){
    if (input != (int)this->state){
        this->state = (State)input;
        since = 0;
    }
    else{
        since += interval;
    }

    return this;
}

Button::Button(int pin)
    : input_pin(pin), state(new ButtonState){
    pinMode(this->input_pin, INPUT_PULLUP);
}

ButtonState* Button::UpdateState(const int interval){
    int input = digitalRead(this->input_pin);
    return this->state->Update(interval, input);
}
