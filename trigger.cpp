// trigger.cpp

#include "trigger.h"

 AggregateOrTrigger::AggregateOrTrigger()
    :_count(0)
 {

 }

bool AggregateOrTrigger::isTriggered()
{
    bool ret = true; 
    for(unsigned int i = 0; i < _count; ++i){
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