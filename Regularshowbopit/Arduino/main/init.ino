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
#define photoPin 14
#define mashButtonPin 4
#define startButtonPin 5
#define indicatorLEDPin 11
#define resetSignalPin 12 //**Note: This pin is used to signal a reset condition, not mapped to physical reset pin */
#define displaySCLPin 28 
#define displaySDAPin 27
#define acclerometerPin 13
#define lpfdPin 1
#define speakerRXPin 2
#define speakerTXPin 3

void gpioInit()
{
        pinMode(photoPin, INPUT);
        pinMode(mashButtonPin, INPUT);
        pinMode(resetSignalPin, OUTPUT);
        pinMode(indicatorLEDPin, OUTPUT);
        pinMode(startButtonPin, INPUT);
        pinMode(displaySCLPin, OUTPUT);
        pinMode(displaySDAPin, OUTPUT);
        pinMode(acclerometerPin, INPUT);
        pinMode(lpfdPin, INPUT);
        pinMode(speakerRXPin, OUTPUT);
        pinMode(speakerTXPin, INPUT);
        digitalWrite(resetSignalPin,HIGH); // set reset signal pin low to start
}

void interruptInit()
{
    attachInterrupt(digitalPinToInterrupt(2),mashButtonISR,LOW);//Enable interrupt for left button
    attachInterrupt(digitalPinToInterrupt(3),startButtonISR,LOW);//Enable interrupt for reset button
   
}



