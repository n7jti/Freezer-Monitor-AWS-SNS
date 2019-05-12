// power
#include <arduino.h>
#include "power.h"

Power::Power(int pin)
  : Trigger()
  , _pin(pin)
{
  
}

bool Power::begin()
{
  pinMode(_pin, INPUT_PULLUP);
  return true; 
}

bool Power::isTriggered()
{
    bool ret = false; 
    int power = analogRead(_pin);

    if (power <= 750)
    {
        //We are on battery power.
        ret = true; 
    }

    return ret; 
}
