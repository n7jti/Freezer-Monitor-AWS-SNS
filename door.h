#pragma once
// door.h

// Door states
enum DOOR_STATE {
  DOOR_GREEN,  // Door is closed
  DOOR_YELLOW, // Door is open less than a minute
  DOOR_RED     // Door is open more than a minute
};

class Door{
public:
  Door();
  void begin(int pin);
  DOOR_STATE run();
  DOOR_STATE getState();
  unsigned int getStateElapsedMs();
private:
   int _pin; 
   DOOR_STATE _state;
   unsigned int _stateStartMs;
};

