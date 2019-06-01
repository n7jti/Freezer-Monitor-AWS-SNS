/*********************************************************************
  Freezer Monitor AWS SNS
  
  Sends an SMS Text message when your freezer door has been left open 
  too long.  This sketch uses a custom circuit board and an adafruit
  feather Huzzah.

  *** DEPENDENCIES ***
  Adfruit Feather Huzzah Libraries (install via board manager) 
    http://arduino.esp8266.com/stable/package_esp8266com_index.json
    https://github.com/esp8266/Arduino
  Arduino NTP Library (install via library manager) 
    https://github.com/arduino-libraries/NTPClient
  RTCLib 
    https://github.com/adafruit/RTClib
  

  *** HARDWARE ***

  Freezer Monitor Circuit Board:
  https://github.com/n7jti/Freezer-Monitor-FeatherWing-PCB

  Adafruit Feather Huzzah:
  https://www.adafruit.com/product/2821

  ***  SOFTWARE AND LICENSES FOR 3RD PARTY CODE ***
  
  Code for this was take from several sources:

  SNS Code taken from the SMS Pet Food Dish.  
  SMS Pet Food Dish example is copyright (c) 2013 Tony DiCola (tony@tonydicola.com)
  and is released under an open source MIT license.  See details at:
  http://opensource.org/licenses/MITlicense
  
  The SHA1 hash and signing code came from the SMS Pet Food Dish example which
  has heritage back to the SHA1 hash and signing code adapted from Peter Knight's 
  library available at https://github.com/Cathedrow/Cryptosuite  

  Feather Huzzah Sample Code from Adafruit (License Unknown)
  https://learn.adafruit.com/adafruit-feather-huzzah-esp8266/using-arduino-ide


  *** SOFTWARE AND LICENSE ***

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  
  Code not covered by the above licenses are covered by the below
  
  (C) Alan Ludwig (2018)
  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
  
*********************************************************************/
#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "config.h"
#include "monitor.h"
#include "notification.h"
#include "door.h"
#include "power.h"
#include "sns.h"

// Freezer Monitor Configuration
#define DOOR_PIN 14
#define RED_PIN 12
#define GREEN_PIN 13
#define BUZZER_PIN 15
#define POWER_SENSE_PIN A0

#define MS_PER_MIN 60000
#define DOOR_TIMEOUT_MS (MS_PER_MIN)

// The configuration below should not be changed.
#define     SHA1_HASH_LENGTH       20

Door *door;
Power *power; 
AggregateOrTrigger trigger; 
Monitor *monitor;
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP,"pool.ntp.org", 0, 12 * 60 * 60 * 1000); //update every 12 hours

Sns *sns;
LedBuzzer *ledBuzzer;

void setup() {
  Serial.begin(115200);
  delay(500); // give a little time for Serial to settle down. 

 
  pinMode(DOOR_PIN, INPUT);
  pinMode(POWER_SENSE_PIN, INPUT);
  digitalWrite(RED_PIN, LOW);    // Low pin turns lights on.
  digitalWrite(GREEN_PIN, LOW);  // Low pin turns lights on

  door = new Door(DOOR_PIN);
  power = new Power(POWER_SENSE_PIN);

  trigger.pushTrigger(door);
  trigger.pushTrigger(power);
  monitor = new Monitor(&trigger, DOOR_TIMEOUT_MS);
  sns = new Sns(*monitor, SNS_TOPIC_ARN);
  ledBuzzer = new ledBuzzer(*monitor, RED_PIN, GREEN_PIN);

  monitor->begin();
  sns->begin(); 
  ledBuzzer->begin(); 

  // attempt to connect to Wifi network:
  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  Serial.println("OK");
}

void loop() { 
  timeClient.update();
  monitor->run();
  ledBuzzer->run(); 
  sns->run(); 
}
