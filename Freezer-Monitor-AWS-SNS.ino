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
#include <RTClib.h>
#include "config.h"
#include "door.h"
#include "sha1.h"

// Freezer Monitor Configuration
#define DOOR_PIN 14
#define RED_PIN 12
#define GREEN_PIN 13
#define BUZZER_PIN 15

#define     TIMEOUT_MS             15000     // How long to wait (in milliseconds) for a server connection to respond (for both AWS and NTP calls).

// The configuration below should not be changed.
#define     SHA1_HASH_LENGTH       20

Door door = Door();
WiFiUDP ntpUDP;

// By default 'pool.ntp.org' is used with 60 seconds update interval and
// no offset
NTPClient timeClient(ntpUDP,"pool.ntp.org", 0, 60 * 60 * 1000); //update once an hour

void setup() {
  Serial.begin(115200);
  delay(500); // give a little time for Serial to settle down. 

  WiFi.mode(WIFI_STA);
  WiFi.begin(WLAN_SSID,WLAN_PASS);
  
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  
  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  
  door.begin(DOOR_PIN);

  digitalWrite(RED_PIN, LOW);    // Low pin turns lights on.
  digitalWrite(GREEN_PIN, LOW);  // Low pin turns lights on
  
  // attempt to connect to Wifi network:

  Serial.println("OK");
}

void loop() { 
  static DOOR_STATE lastDoorState = DOOR_GREEN;
  DOOR_STATE doorState = door.run();
  timeClient.update();

  if (doorState == DOOR_GREEN)
  {
    analogWrite(BUZZER_PIN, 0);
    digitalWrite(RED_PIN, HIGH);  // OFF
    digitalWrite(GREEN_PIN, LOW); // ON
  }
  else if (doorState == DOOR_YELLOW) // red LED
  {
    digitalWrite(RED_PIN, LOW);    // ON
    digitalWrite(GREEN_PIN, HIGH); // OFF
  }
  else // DOOR_RED -- Flashing red LED
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
  }
 
  // return if the value hasn't changed
  if(doorState == lastDoorState)
    return;

  // state changed!
  // timer.start(); // restart the timer if our state changed.  We don't want to disconnect if we have been recently active. 
  if (DOOR_GREEN == doorState)
  {
    // We're Green Again
    if (lastDoorState == DOOR_RED)
    {
      //Silence the Alarm
      snsPublish(SNS_TOPIC_ARN, "Freezer%20is%20CLOSED");
    }
  }
  else if (DOOR_RED == doorState)
  { 
    // Alarm!
    analogWrite(BUZZER_PIN, 50); 
    snsPublish(SNS_TOPIC_ARN, "Freezer%20is%20OPEN");
  }
  else // (DOOR_YELLOW == doorState)
  {
    // Door Open
  }
  
  lastDoorState = doorState;
}

// Publish a message to an SNS topic.
// Note, both the topic and message strings _MUST_ be URL encoded before calling this function!
void snsPublish(const char* topic, const char* message) {
  // Set dateTime to the URL encoded ISO8601 format string.
  DateTime dt(timeClient.getEpochTime());
  char dateTime[25];
  memset(dateTime, 0, 25);
  dateTime8601UrlEncoded(dt.year(), dt.month(), dt.day(), dt.hour(), dt.minute(), dt.second(), dateTime);

  // Generate the signature for the request.
  // For details on the AWS signature process, see: 
  //   http://docs.aws.amazon.com/general/latest/gr/signature-version-2.html
  Sha1.initHmac((uint8_t*)AWS_SECRET_ACCESS_KEY, strlen(AWS_SECRET_ACCESS_KEY));
  Sha1.print(F("POST\n"));
  Sha1.print(AWS_HOST); Sha1.print(F("\n"));
  Sha1.print(F("/\n"));
  Sha1.print(F("AWSAccessKeyId="));
  Sha1.print(AWS_ACCESS_KEY);
  Sha1.print(F("&Action=Publish"));
  Sha1.print(F("&Message="));
  Sha1.print(message);
  Sha1.print(F("&SignatureMethod=HmacSHA1"));
  Sha1.print(F("&SignatureVersion=2"));
  Sha1.print(F("&Timestamp="));
  Sha1.print(dateTime);
  Sha1.print(F("&TopicArn="));
  Sha1.print(topic);
  Sha1.print(F("&Version=2010-03-31"));
  
  // Convert signature to base64
  // Adapted from Adafruit code for SendTweet example.
  uint8_t *in, out, i, j;
  char b64[27];
  memset(b64, 0, sizeof(b64));
  static const char PROGMEM b64chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
  for(in = Sha1.resultHmac(), out=0; ; in += 3) { // octets to sextets
    b64[out++] =   in[0] >> 2;
    b64[out++] = ((in[0] & 0x03) << 4) | (in[1] >> 4);
    if(out >= 26) break;
    b64[out++] = ((in[1] & 0x0f) << 2) | (in[2] >> 6);
    b64[out++] =   in[2] & 0x3f;
  }
  b64[out] = (in[1] & 0x0f) << 2;
  // Remap sextets to base64 ASCII chars
  for(i=0; i<=out; i++) b64[i] = pgm_read_byte(&b64chars[(unsigned char)b64[i]]);
  
  // URL encode base64 signature.  Note, this is not a general URL encoding routine!
  char b64Encoded[100];
  memset(b64Encoded, 0, sizeof(b64Encoded));
  for(i=0, j=0; i<=out; i++) {
    uint8_t ch = b64[i];
    if (ch == '+') {
      b64Encoded[j++] = '%';  
      b64Encoded[j++] = '2';  
      b64Encoded[j++] = 'B';  
    }
    else if (ch == '/') {
      b64Encoded[j++] = '%';  
      b64Encoded[j++] = '2';  
      b64Encoded[j++] = 'F'; 
    }
    else {
      b64Encoded[j++] = ch;
    }
  }
  b64Encoded[j++] = '%';
  b64Encoded[j++] = '3';
  b64Encoded[j++] = 'D';
  
  // Make request to SNS API.

  WiFiClient www;

  while(!www.connect(AWS_HOST, 80))
  {
    Serial.println("connection to AWS_HOST failed");
    delay(500);
  }
  
  if (www.connected()) {    
    www.print(F("POST /?"));
    www.print(F("AWSAccessKeyId="));
    www.print(AWS_ACCESS_KEY);
    www.print(F("&Action=Publish"));
    www.print(F("&Message="));
    www.print(message);
    www.print(F("&SignatureMethod=HmacSHA1"));
    www.print(F("&SignatureVersion=2"));
    www.print(F("&Timestamp="));
    www.print(dateTime);
    www.print(F("&TopicArn="));
    www.print(topic);
    www.print(F("&Version=2010-03-31"));
    www.print(F("&Signature="));
    www.print(b64Encoded);  
    www.print(F(" HTTP/1.1\r\nHost: "));
    www.print(AWS_HOST);
    www.print(F("\r\nContent-Length: 0\r\n\r\n"));
  } 
  else {
    Serial.println(F("Connection failed"));    
    www.stop();
    return;
  }
  
  // Read data until either the connection is closed, or the idle timeout is reached.
  Serial.println(F("AWS response:"));
  unsigned long lastRead = millis();
  while (www.connected() && (millis() - lastRead < TIMEOUT_MS)) {
    while (www.available()) {
      char c = www.read();
      Serial.print(c);
      lastRead = millis();
    }
  }
  www.stop();
}

// Fill a 24 character buffer with the date in URL-encoded ISO8601 format, like '2013-01-01T01%3A01%3A01Z'.  
// Buffer MUST be at least 24 characters long!
void dateTime8601UrlEncoded(int year, byte month, byte day, byte hour, byte minute, byte seconds, char* buffer) {
  ultoa(year, buffer, 10);
  buffer[4] = '-';
  btoa2Padded(month, buffer+5, 10);
  buffer[7] = '-';
  btoa2Padded(day, buffer+8, 10);
  buffer[10] = 'T';
  btoa2Padded(hour, buffer+11, 10);
  buffer[13] = '%';
  buffer[14] = '3';
  buffer[15] = 'A';
  btoa2Padded(minute, buffer+16, 10);
  buffer[18] = '%';
  buffer[19] = '3';
  buffer[20] = 'A';
  btoa2Padded(seconds, buffer+21, 10);
  buffer[23] = 'Z';
}

// Print a value from 0-99 to a 2 character 0 padded character buffer.
// Buffer MUST be at least 2 characters long!
void btoa2Padded(uint8_t value, char* buffer, int base) {
  if (value < base) {
    *buffer = '0';
    ultoa(value, buffer+1, base);
  }
  else {
    ultoa(value, buffer, base); 
  }
}
