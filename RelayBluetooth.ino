

#include <CurieBLE.h>
#define buzzer 4 //buzzer Pin
#define echoPin 9 // Echo Pin
#define trigPin 8 // Trigger Pin
#define LEDPin 13 // Onboard LED


// NeoPixel Ring simple sketch (c) 2013 Shae Erisson
// released under the GPLv3 license to match the rest of the AdaFruit NeoPixel library




int maximumRange = 200; // Maximum range needed
int minimumRange = 0; // Minimum range needed
long duration, distance, lastdistance; // Duration used to calculate distance
boolean awayMode = false;

BLEService ledService("19B10010-E8F2-537E-4F6C-D104768A1214"); // BLE LED Service

// BLE LED Switch Characteristic - custom 128-bit UUID, read and writable by central
BLEUnsignedCharCharacteristic switchCharacteristic("19B10010-E8F2-537E-4F6C-D104768A1214", BLERead | BLEWrite);

const int ledPin = 7; // pin to use for the LED

void setup() {
  Serial.begin(9600);

  // set LED pin to output mode
  pinMode(buzzer, OUTPUT);
  pinMode(ledPin, OUTPUT);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  pinMode(LEDPin, OUTPUT); // Use LED indicator (if required)
  pinMode(3,OUTPUT);
  digitalWrite(3, HIGH);
  // begin initialization
  BLE.begin();

  // set advertised local name and service UUID:
  BLE.setLocalName("LED");
  BLE.setAdvertisedService(ledService);

  // add the characteristic to the service
  ledService.addCharacteristic(switchCharacteristic);

  // add service
  BLE.addService(ledService);

  // set the initial value for the characeristic:
  switchCharacteristic.setValue(0);

  // start advertising
  BLE.advertise();

  Serial.println("BLE LED Peripheral");





  
}

void loop() {
  // listen for BLE peripherals to connect:
  BLEDevice central = BLE.central();

  // if a central is connected to peripheral:
   if (central) {
    Serial.print("Connected to central: ");
    // print the central's MAC address:
    Serial.println(central.address());

    // while the central is still connected to peripheral:
    while (central.connected()) {
       digitalWrite(3, HIGH);
       
       digitalWrite(trigPin, LOW); 
       delayMicroseconds(2); 
      
       digitalWrite(trigPin, HIGH);
       delayMicroseconds(10); 
       digitalWrite(trigPin, LOW);
      
       duration = pulseIn(echoPin, HIGH);
       
       //Calculate the distance (in cm) based on the speed of sound.
       distance = duration/58.2;
       
       if (distance >= maximumRange || distance <= minimumRange){
       /* Send a negative number to computer and Turn LED ON 
       to indicate "out of range" */
       Serial.println("-1");
       digitalWrite(LEDPin, HIGH); 
       }
       else {
       /* Send the distance to the computer using Serial protocol, and
       turn LED OFF to indicate successful reading. */
       Serial.println(distance);
       digitalWrite(LEDPin, LOW); 
       }
        Serial.println(awayMode);

       if (awayMode == true){
        digitalWrite(ledPin, LOW);  
        Serial.println("Alarm Active");
          if(distance <150 && distance >0)
          {
            delay(500);
            digitalWrite(3, LOW);
            delay(500);
            digitalWrite(3, HIGH);
            delay(500);
            digitalWrite(3, LOW);
            delay(500);
            digitalWrite(3, HIGH);

            digitalWrite(buzzer, HIGH);   // turn the LED on (HIGH is the voltage level)

            delay(500);
            digitalWrite(3, LOW);
            delay(500);
            digitalWrite(3, HIGH);
            delay(500);
            digitalWrite(3, LOW);
            delay(500);
            digitalWrite(3, HIGH);


            Serial.println("Alarm on");
            digitalWrite(ledPin, HIGH);  
            awayMode = false;
          }
        }

        

      // if the remote device wrote to the characteristic,
      // use the value to control the LED:
      if (switchCharacteristic.written()) {
        if (switchCharacteristic.value()==49) {   // any value other than 0
          Serial.println("LED on");
          digitalWrite(buzzer, LOW);
          digitalWrite(ledPin, HIGH);         // will turn the LED on
          awayMode = false;
        } 
        else if (switchCharacteristic.value()==48) {                              // a 0 value
          Serial.println(F("LED off"));
          digitalWrite(buzzer, LOW);
          digitalWrite(ledPin, LOW);          // will turn the LED off
          awayMode = false;
        } 
        else if (switchCharacteristic.value()==50) {                              // a 0 value
          Serial.println(F("LED off"));
          Serial.println("Away Mode");
          digitalWrite(ledPin, LOW);          // will turn the LED off
          delay(10);
          awayMode = true;
          
        }

      }

    }

    // when the central disconnects, print it out:
    Serial.print(F("Disconnected from central: "));
    Serial.println(central.address());
  }
}

/*
   Copyright (c) 2016 Intel Corporation.  All rights reserved.

   This library is free software; you can redistribute it and/or
   modify it under the terms of the GNU Lesser General Public
   License as published by the Free Software Foundation; either
   version 2.1 of the License, or (at your option) any later version.

   This library is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   Lesser General Public License for more details.

   You should have received a copy of the GNU Lesser General Public
   License along with this library; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA
*/
