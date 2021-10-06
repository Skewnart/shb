#ifndef button_h
#define button_h

#include <Arduino.h>

enum State{
    PRESSED,
    RELEASED
};

class ButtonState {

    public :
        State state;
        long since;
        
        ButtonState();
        ButtonState* Update(const int, const int);
};

class Button {

    private :
        int input_pin;

    public :
        ButtonState* state;

        Button(int);
        ButtonState* UpdateState(const int);
};

#endif
