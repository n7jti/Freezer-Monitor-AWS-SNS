#pragma once

// door.h

#include "trigger.h"

class Door : public Trigger{
public:
  Door(int pin);
  virtual bool begin();
  virtual bool isTriggered();
  virtual int getStatus(char* buffer, int length);
private:
   int _pin; 
};

