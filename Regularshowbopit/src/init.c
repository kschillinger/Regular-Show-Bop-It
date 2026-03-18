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
    attachInterrupt(digitalPinToInterrupt(2),leftButtonISR,FALLING);//Enable interrupt for left button
    attachInterrupt(digitalPinToInterrupt(3),rightButtonISR,FALLING);//Enable interrupt for right button
    attachPCINT(digitalPinToPCINT(4),startButtonISR,FALLING);//Enable interrupt for left button
}


