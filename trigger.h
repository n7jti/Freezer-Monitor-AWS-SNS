#pragma once 

// trigger.h
#define MAX_TRIGGERS 2

class Trigger{
public:
    virtual bool isTriggered() = 0;
    virtual bool begin() = 0;
    virtual int getStatus(char* buffer, int length) = 0;
};

// class that aggregates up to MAX_TRIGGERS triggers
// isTriggered() aggregates by doing a logical OR of all 
// triggers.  Just to be cute you could notice that by adding
// a NOT wrapper and an AND aggregator we could build up any sort 
// of arbitarily complex logical function to aggregate any set of 
// triggers.  A little too abstract to be truly useful, but cute 
// anyway. 

class AggregateOrTrigger : public Trigger
{
public:
    // Constructors
    AggregateOrTrigger();

    // Trigger methods
    virtual bool isTriggered(); 
    virtual bool begin();
    virtual int getStatus(char* buffer, int length);

    // Add triggers to aggregate.  We are an embedded platform and we
    // are never going to tear-down so no need for the rest of the
    // operations it takes to tear-down. 
    bool pushTrigger(Trigger *trigger);  

private: 
    Trigger *_triggers[MAX_TRIGGERS]; // trigger array
    unsigned int _count;              // count of triggers, 0 to MAX_TRIGGERS
};