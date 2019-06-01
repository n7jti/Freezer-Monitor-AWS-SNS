#pragma once

// notification.h
#include "monitor.h"

class Notification {
public:
    Notification(Monitor &monitor);
    virtual bool begin();
    virtual void run();
protected:
    virtual void onRed(bool isNew) = 0;
    virtual void onYellow(bool isNew) = 0;
    virtual void onGreen(bool isNew) = 0; 

    MONITOR_STATE _lastState; 
    Monitor &_monitor;

};