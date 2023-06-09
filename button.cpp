#include "button.h"

ButtonState::ButtonState()
    : current(RELEASED), previous(RELEASED), since(0){
}

ButtonState* ButtonState::Update(const int interval, const int input){
    this->previous = this->current;
    if (input != (int)this->current){
        this->current = (State)input;
        this->since = 0;
    }
    else{
        this->since += interval;
    }

    return this;
}

Button::Button(const int& pin, const int& interval)
    : input_pin(pin), update_interval(interval), state(new ButtonState){
    pinMode(this->input_pin, INPUT_PULLUP);
}

ButtonState* Button::UpdateState(const int interval){
    int input = digitalRead(this->input_pin);
    return this->state->Update(interval, input);
}
