// trigger.cpp

#include <Arduino.h>
#include "trigger.h"

 AggregateOrTrigger::AggregateOrTrigger()
    :_count(0)
 {

 }

bool AggregateOrTrigger::isTriggered()
{
    bool ret = false; 
    for(unsigned int i = 0; i < _count; ++i){
       bool flag; 
       ret = ret || _triggers[i]->isTriggered(); 
    }
    return ret; 
}

bool AggregateOrTrigger::begin()
{
    bool ret = true; 
    for(unsigned int i = 0; i < _count; ++i){
        ret = ret && _triggers[i]->begin(); 
    }

    return ret; 
}

bool AggregateOrTrigger::pushTrigger(Trigger *trigger)
{
    if (_count == MAX_TRIGGERS){
        return false; 
    }

    _triggers[_count] = trigger; 
    _count += 1; 
    return true; 
}

int AggregateOrTrigger::getStatus(char* buffer, int length)
{
    int len;
    int total = 0;  
    for(unsigned int i = 0; i < _count; ++i){
       len =  _triggers[i]->getStatus(buffer, length);
       buffer += len; 
       length -= len; 
       total += len; 
    }

    return total; 
}