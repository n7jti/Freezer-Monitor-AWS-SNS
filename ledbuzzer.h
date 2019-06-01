#pragma once

// ledbuzzer.h

#include "notification.h"

class LedBuzzer : public Notification
{
public:
    LedBuzzer(Monitor &monitor, int redpin, int greenpin);
    virtual bool begin();
    void run();
protected:
    virtual void onRed(bool isNew);
    virtual void onYellow(bool isNew);
    virtual void onGreen(bool isNew); 
private:
    int _redpin; 
    int _greenpin;

    

};