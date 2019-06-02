// monitor.cpp
#include <arduino.h>
#include "monitor.h"

Monitor::Monitor(Trigger *trigger, unsigned int timeout) 
    : _trigger(trigger)
    , _timeout(timeout)
    , _state(MONITOR_GREEN)
    , _stateStartMs(millis())
{

}

bool Monitor::begin()
{
    return _trigger->begin();  
}

MONITOR_STATE Monitor::getState()
{
  return _state;
}

unsigned int Monitor::getStateElapsedMs()
{
  return millis() - _stateStartMs;
}

MONITOR_STATE Monitor::run()
{
    unsigned int now = millis();
    unsigned int elapsed = now - _stateStartMs;
    bool triggered = _trigger->isTriggered(); 
  
    switch(_state)
    {
    case MONITOR_RED:
        if (triggered == false) 
        {
            _stateStartMs = now;
            _state = MONITOR_GREEN;
        }
        break;
    case MONITOR_YELLOW:
        if (triggered == false) 
        {
            _stateStartMs = now;
            _state = MONITOR_GREEN;
        }
        else if (elapsed > _timeout) 
        {
            _state = MONITOR_RED;
        }
        break;
    case MONITOR_GREEN:
        if (triggered == true) 
        {
            _stateStartMs = now;
            _state = MONITOR_YELLOW;
        }
        break;
    }

    return _state;
}

int Monitor::getStatus(char* buffer, int length)
{
    return _trigger->getStatus(buffer, length);
}