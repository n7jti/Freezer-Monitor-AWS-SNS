// ledbuzzer.cpp
#include <Arduino.h>
#include "ledbuzzer.h"

LedBuzzer::LedBuzzer(Monitor &monitor, int redpin, int greenpin, int buzzerpin) 
    : Notification(monitor)
    , _redpin(redpin)
    , _greenpin(greenpin)
    , _buzzerpin(buzzerpin)
{

}

bool LedBuzzer::begin()
{
    pinMode(_redpin, OUTPUT);
    pinMode(_greenpin, OUTPUT);

}

void LedBuzzer::run()
{
    Notification::run();
}

void LedBuzzer::onRed(bool isNew)
{
    unsigned int ms = millis();
    if ((ms % 1000) < 500)
    {
      digitalWrite(_redpin, LOW); // ON
    }
    else
    {
      digitalWrite(_redpin, HIGH); // OFF
    }
    digitalWrite(_greenpin, HIGH); // OFF

    // Alarm!
    if (isNew) {
        analogWrite(_buzzerpin, 50);
    } 
}

void LedBuzzer::onYellow(bool isNew)
{
    digitalWrite(_redpin, LOW);  // ON
    digitalWrite(_greenpin, HIGH); // OFF
}

void LedBuzzer::onGreen(bool isNew)
{
    analogWrite(_buzzerpin, 0);
    digitalWrite(_redpin, HIGH);  // OFF
    digitalWrite(_greenpin, LOW); // ON
}
