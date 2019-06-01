//  SNS Code taken from the SMS Pet Food Dish.  
//  SMS Pet Food Dish example is copyright (c) 2013 Tony DiCola (tony@tonydicola.com)
//  and is released under an open source MIT license.  See details at:
//  http://opensource.org/licenses/MITlicense

#include <NTPClient.h>
#include <ESP8266WiFi.h>
#include <RTClib.h>
#include "sha1.h"
#include "sns.h"
#include "config.h"

Sns::Sns(Monitor &monitor, NTPClient &timeClient, const char* topic)
    : Notification(monitor)
    , _timeClient(timeClient)
    , _topic(topic)
    , _isAlerted(false)
{

}

bool Sns::begin()
{

}

void Sns::run()
{
    Notification::run(); 
}

void Sns::onRed(bool isNew)
{
  if (isNew){
    snsPublish(_timeClient, _topic, "Freezer%20is%20OPEN");
    _isAlerted = true; 
  }
}

void Sns::onYellow(bool isNew)
{
    

}

void Sns::onGreen(bool isNew)
{
  if(_isAlerted)
  {
    //Silence the Alarm
    snsPublish(_timeClient, _topic, "Freezer%20is%20CLOSED");
    _isAlerted = false; 
  }
}



// forward declarations
void btoa2Padded(uint8_t value, char* buffer, int base);
void dateTime8601UrlEncoded(int year, byte month, byte day, byte hour, byte minute, byte seconds, char* buffer);

// Publish a message to an SNS topic.
// Note, both the topic and message strings _MUST_ be URL encoded before calling this function!
void snsPublish(NTPClient &timeClient, const char* topic, const char* message) {
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
