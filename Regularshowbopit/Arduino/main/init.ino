#include "init.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>

//Pin definitions

#define photoPin 8         //phys 14
#define mashButtonPin 2    //pys 4
#define startButtonPin 3   //phys5
#define indicatorLEDPin 5  //phys11
#define resetSignalPin 6   //phys12
#define displaySCLPin 28   //analog 5
#define displaySDAPin 27   //analog 4
#define acclerometerPin 7  //phys 13
//#define lpfdPin 1
#define speakerRXPin 0  //phys2
#define speakerTXPin 1  //phys3


void gpioInit() {

  pinMode(photoPin, INPUT);
  pinMode(mashButtonPin, INPUT);
  pinMode(resetSignalPin, OUTPUT);
  pinMode(indicatorLEDPin, OUTPUT);
  pinMode(startButtonPin, INPUT);
  pinMode(displaySCLPin, OUTPUT);
  pinMode(displaySDAPin, OUTPUT);
  pinMode(acclerometerPin, INPUT);
  //pinMode(lpfdPin, INPUT);
  pinMode(speakerRXPin, OUTPUT);
  pinMode(speakerTXPin, INPUT);
  digitalWrite(resetSignalPin, HIGH);  // set reset signal pin low to start
}

void interruptInit() 
{

  attachInterrupt(digitalPinToInterrupt(2), mashButtonISR, RISING);  //Enable interrupt for left button

  attachInterrupt(digitalPinToInterrupt(3), startButtonISR, RISING);  //Enable interrupt for reset button
}
