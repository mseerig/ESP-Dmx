// - - - - -
// ESPDMX - A Arduino library for sending DMX signals using the built-in serial hardware port.
// ESPDMX.cpp: Library implementation file
//
// Copyright (C) 2015  Rick <ricardogg95@gmail.com>
// This work is licensed under a GNU style license. 
//
// Last change: Marcel Seerig <https://github.com/mseerig>
//
// more information: https://github.com/Rickgg/ESP-Dmx
// - - - - -

/* ----- LIBRARIES ----- */
#include <Arduino.h>
#include "ESPDMX.h"

#define dmxMaxChannel  512

#define DMXSPEED       250000
#define DMXFORMAT      SERIAL_8N2

bool dmxStarted = false;
int sendPin = 2;		//dafault on ESP8266

//DMX value array. Entry 0 will hold startbyte
uint8_t dmxData[dmxMaxChannel + 1];

DMXESPSerial DMXSerial;

// Set up the DMX-Protocol
void DMXESPSerial::init() {
  //Initialize buffer with zero
  for (int iR=0; iR < dmxMaxChannel; iR++) {
    dmxData[iR] = 0;
  }
  Serial1.begin(DMXSPEED, DMXFORMAT);
  pinMode(sendPin, OUTPUT);
  dmxStarted = true;
}

// Change Serial1 Pin if TX Pin is not Pin 2
void DMXESPSerial::setSendPin(int _pin) {
  if (dmxStarted == false) init();
  sendPin = _pin;
}
/*
// Function to read DMX data
uint8_t DMXESPSerial::read(int Channel) {
  if (dmxStarted == false) init();
  
  if (Channel < 1) Channel = 1;
  if (Channel > dmxMaxChannel) Channel = dmxMaxChannel;
  return(dmxData[Channel]);
}
*/

// Function to send DMX data
void DMXESPSerial::write(int Channel, uint8_t value) {
  if (dmxStarted == false) init();
  
  if (Channel < 1) Channel = 1;
  if (Channel > dmxMaxChannel) Channel = dmxMaxChannel;
  if (value < 0) value = 0;
  if (value > 255) value = 255;

  dmxData[Channel] = value;
}

// Function to update the DMX bus
void DMXESPSerial::update() {
  if (dmxStarted == false) init();
  
  //Send break
  analogWrite(sendPin, 0);
  delay(1);
  analogWrite(sendPin, 255);
  delay(1);
  
  //send data
  digitalWrite(sendPin, LOW);
  Serial1.write(dmxData, sizeof(dmxData));
  Serial1.flush();
}
