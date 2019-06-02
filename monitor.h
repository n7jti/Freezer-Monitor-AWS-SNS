#pragma once

// monitor.h

#include "trigger.h"

// Door states
enum MONITOR_STATE {
  MONITOR_GREEN,  // The monitor is Green
  MONITOR_YELLOW, // The monitor has been tripped, but for less than the time out
  MONITOR_RED     // The monitor has been tripped, for longer than the time out. 
};

class Monitor{
public:
    Monitor(Trigger *trigger, unsigned int timeout);
    bool begin(); 
    MONITOR_STATE run();
    MONITOR_STATE getState();
    unsigned int getStateElapsedMs();
    int getStatus(char* buffer, int length);
private:
    MONITOR_STATE _state;
    unsigned int _stateStartMs;
    unsigned int _timeout; 
    Trigger *_trigger; 
};

