// door
#include <arduino.h>
#include "door.h"

Door::Door(int pin)
  : Trigger()
  , _pin(pin)
{
  
}

bool Door::begin()
{
  pinMode(_pin, INPUT_PULLUP);
  return true; 
}

bool Door::isTriggered()
{
  // The magnetic switch connects the pin to ground.
  // The magnetic switch is normally open, but is pulled closed by the magnet.
  // The pin is pulled-up and reads high when the switch is open 
  // We want our state variables to be HIGH when the door is open and LOW when closed. 
  // The pin reads high when the door is open. 
  int doorIn = digitalRead(_pin);
  return (doorIn == HIGH);
}

