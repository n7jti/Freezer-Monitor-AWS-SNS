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
  bool ret;
  // The magnetic switch connects the pin to ground.
  // The magnetic switch is normally open, but is pulled closed by the magnet.
  // The pin is pulled-up and reads high when the switch is open 
  // We want our state variables to be HIGH when the door is open and LOW when closed. 
  // The pin reads high when the door is open. 
  return digitalRead(_pin) == HIGH;
}

int Door::getStatus(char* buffer, int length)
{
  int len; 
  constexpr char msgOpen[] = "Freezer%20is%20OPEN%21%0A";
  constexpr char msgClosed[] = "Freezer%20is%20CLOSED.%0A";
  if (isTriggered()) {
    strncpy(buffer, msgOpen, length);
    len = strlen(msgOpen);
    if (length < len){
      len = length; 
    }
  }
  else {
    // copy Freezer is Closed; 
    strncpy(buffer, msgClosed, length);
    len = strlen(msgClosed);
    if (length < len){
      len = length; 
    }
  }
  return len; 
}

