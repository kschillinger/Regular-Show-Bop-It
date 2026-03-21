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
#define leftButtonPin 4
#define rightButtonPin 5
#define startButtonPin 6
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
        pinMode(leftButtonPin, INPUT);
        pinMode(rightButtonPin, INPUT);
        pinMode(resetSignalPin, OUTPUT);
        pinMode(indicatorLEDPin, OUTPUT);
        pinMode(startButtonPin, INPUT);
        pinMode(displaySCLPin, OUTPUT);
        pinMode(displaySDAPin, OUTPUT);
        pinMode(acclerometerPin, INPUT);
        pinMode(lpfdPin, INPUT);
        pinMode(speakerRXPin, OUTPUT);
        pinMode(speakerTXPin, INPUT);
}

void interruptInit()
{
    attachInterrupt(digitalPinToInterrupt(2),mashButtonISR,FALLING);//Enable interrupt for left button
    attachInterrupt(digitalPinToInterrupt(4),startButtonISR,FALLING);//Enable interrupt for reset button
   
}


