#pragma once

// power.h

#include "trigger.h"

// The count at which we assume that we have lost mains power.
// 1v in is 1024 count (10 bits). 
// We have a voltage divider on the input 10,000 & 2,200.  that makes 1v = 0.1803v.  So we get a count of 1023 at 5.546v.  
// so 4v would be about 739, call it 750 (or 4.06v)

#define POWER_THRESHOLD 750

class Power : public Trigger{
public:
  Power(int pin);
  virtual bool begin();
  virtual bool isTriggered();
private:
   int _pin; 
};
