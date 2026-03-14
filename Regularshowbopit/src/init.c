#include "init.h"
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include<avr/interrupt.h>

void gpioInit()
{
}

void interruptInit()
{
    attachInterrupt(0,leftButtonISR(),FALLING);//Enable interrupt for left button
    attachInterrupt(1,righButtonISR(),FALLING);//Enable interrupt for right button
    attachPCINT(digitalPinToPCINT(2),startButtonISR(),FALLING);//Enable interrupt for left button
}


