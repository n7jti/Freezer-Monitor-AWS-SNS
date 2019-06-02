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

int Power::getStatus(char* buffer, int length)
{
  int len;  
  constexpr char msgTriggered[] = "Power%20is%20OFF%21%0A";
  constexpr char msgNotTriggered[] = "Power%20is%20ON.%0A";
  if (isTriggered()) {
    strncpy(buffer, msgTriggered, length);
    len = strlen(msgTriggered);
    if (length < len){
      len = length; 
    }
  }
  else {
    // copy Freezer is Closed; 
    strncpy(buffer, msgNotTriggered, length);
    len = strlen(msgNotTriggered);
    if (length < len){
      len = length; 
    }
  }
  return len; 
}
