/*
  Copyright © 2018 Razvan Murariu <razwww@gmail.com>
  This program is free software. It comes without any warranty, to
  the extent permitted by applicable law. You can redistribute it
  and/or modify it under the terms of the Do What The Fuck You Want
  To Public License, Version 2, as published by Sam Hocevar. See
  the COPYING file for more details.

  The circuit:
  Water pump: D7
  Sensor (1 MO resistor): A0
*/

#include <LowPower.h>

#define sensorPin A0

byte pumpPin=4;
int tempo=6000;  //watering time
int i=0;  //counter

int readSensor() {
  byte readCount = 0;
  int sensorValueTotal = 0;
  int sensorValueAveraged = 0;
  digitalWrite(sensorPin,HIGH); // to avoid electrocorrosion we set it to high only during measuring
  
  while (readCount < 5) {
    int sensorValue = analogRead(sensorPin);
//    Serial.println(sensorValue);
    sensorValueTotal += sensorValue;
    readCount++;
    delay(10);
  }
  
  sensorValueAveraged = sensorValueTotal / 5;
  digitalWrite(sensorPin,LOW);
//  Serial.println(sensorValueAveraged);
  return (sensorValueAveraged);
}

void setup() {
  Serial.begin(9600);
  pinMode(pumpPin,OUTPUT);
  pinMode(sensorPin,INPUT);
  pinMode(13,OUTPUT);
  digitalWrite(13,LOW); //turn off led
  digitalWrite(pumpPin,LOW);  //turn off pump
}

void loop() {
  if( i==0 && readSensor()>900 ) { // dry=982
    digitalWrite(pumpPin,HIGH);
    delay(tempo);
    digitalWrite(pumpPin,LOW);
//    Serial.println("pompa");
  }
  
  i++;
  // Enter power down state for X s with ADC and BOD module disabled
  LowPower.powerDown(SLEEP_8S, ADC_OFF, BOD_OFF);

  if( i==5400 ) {  // 10800 = once per day
    i=0; // reset i
  }          
}
