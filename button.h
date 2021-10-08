#ifndef button_h
#define button_h

#include <Arduino.h>

enum State{
    PRESSED,
    RELEASED
};

class ButtonState {

    public :
        State previous;
        State current;
        long since;
        
        ButtonState();
        ButtonState* Update(const int, const int);
};

class Button {

    private :
        int input_pin;
        int update_interval;

    public :
        ButtonState* state;

        Button(const int&, const int&);
        ButtonState* UpdateState(const int);
};

#endif
