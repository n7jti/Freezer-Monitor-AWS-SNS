# Freezer-Monitor-AWS-SNS
Freezer Monitor using AWS Simple Notification Service to send text messages when the freezer is left open
This sketch uses a custom circuit board and an adafruit
  feather Huzzah.

## DEPENDENCIES 
- Adfruit Feather Huzzah Libraries (install via board manager) 
    - http://arduino.esp8266.com/stable/package_esp8266com_index.json
    - https://github.com/esp8266/Arduino
 - Arduino NTP Library (install via library manager) 
    - https://github.com/arduino-libraries/NTPClient
-  RTCLib 
    - https://github.com/adafruit/RTClib
  
## HARDWARE
- Freezer Monitor Circuit Board: https://github.com/n7jti/Freezer-Monitor-FeatherWing-PCB
- Adafruit Feather Huzzah: https://www.adafruit.com/product/2821

## SOFTWARE AND LICENSES FOR 3RD PARTY CODE 
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


## SOFTWARE AND LICENSE

  Adafruit invests time and resources providing this open source code,
  please support Adafruit and open-source hardware by purchasing
  products from Adafruit!
  
  Code not covered by the above licenses are covered by the below
  
  (C) Alan Ludwig (2018)
  MIT license, check LICENSE for more information
  All text above, and the splash screen below must be included in
  any redistribution
