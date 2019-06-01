// ledbuzzer.cpp
#include <arduino.h>
#include "ledbuzzer.h"

LedBuzzer::LedBuzzer(Monitor &monitor, int redpin, int greenpin) 
    : Notification(monitor)
    , _redpin(redpin)
    , _greenpin(greenpin)
{

}

bool LedBuzzer::begin()
{
    pinMode(_redpin, OUTPUT);
    pinMode(_greenpin, OUTPUT);

}

void LedBuzzer::run()
{
    Notifiction::run()
}

void LedBuzzer::onRed(bool isNew)
{
    unsigned int ms = millis();
    if ((ms % 1000) < 500)
    {
      digitalWrite(RED_PIN, LOW); // ON
    }
    else
    {
      digitalWrite(RED_PIN, HIGH); // OFF
    }
    digitalWrite(GREEN_PIN, HIGH); // OFF

    // Alarm!
    if (isNew) {
        analogWrite(BUZZER_PIN, 50);
    }
}

void LedBuzzer::onYellow(bool isNew)
{
    digitalWrite(RED_PIN, LOW);    // ON
    digitalWrite(GREEN_PIN, HIGH); // OFF
}

void LedBuzzer::onGreen(bool isNew)
{
    analogWrite(BUZZER_PIN, 0);
    digitalWrite(RED_PIN, HIGH);  // OFF
    digitalWrite(GREEN_PIN, LOW); // ON
}
