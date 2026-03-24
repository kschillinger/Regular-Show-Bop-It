/*
Main File for bop it game.
Last Updated: 3/24/2026
Rewritten for Arduino IDE using setup()/loop()
*/

#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include <avr/interrupt.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Sensor.h>
#include <SoftwareSerial.h>
#include "speakerConfig.hpp"
#include "displayConfig.hpp"

extern "C"
{
#include "init.h"
}

// Globals
#define REQUIRED_COUNT 5
#define GLOBAL_DEL 5000
#define SHAKE_THRESH 16384

// Pin definitions
#define photoPin 8 //phys 14
#define mashButtonPin 2 //pys 4
#define startButtonPin 3 //phys5
#define indicatorLEDPin 5 //phys11
#define resetSignalPin 6 //phys12
#define displaySCLPin 28 //analog 5
#define displaySDAPin 27 //analog 4
#define acclerometerPin 7 //phys 13
//#define lpfdPin 1
#define speakerRXPin 0//phys2 
#define speakerTXPin 1//phys3

// State variables
uint8_t currentState = prestart;
volatile uint16_t mashbuttonCount = 0;  // volatile - modified in ISR
uint8_t score = 0x00;
uint16_t delayms = GLOBAL_DEL;
uint8_t increment = GLOBAL_DEL / 100;

// Peripherals
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
LiquidCrystal_PCF8574 lcd(0x27);

// ISRs - interrupts are already disabled when an ISR runs,
// so no need for cli()/sei() inside them
void mashButtonISR()
{
     mashbuttonCount++;
}

void startButtonISR()
{
     detachInterrupt(digitalPinToInterrupt(3));
     if (score > 0)
     {
          digitalWrite(resetSignalPin, LOW);
     }
     else
     {
          sleep_disable();
     }
      attachInterrupt(digitalPinToInterrupt(3),startButtonISR,LOW);
}

// Hardware reads
bool readPhotoRes(void)
{
     return (digitalRead(photoPin) == HIGH);
}

// Helpers
void updateDelay()
{
     if (delayms <= increment)
     {
          delayms = 200;
     }
     else
     {
          delayms -= increment;
     }
}

void setup()
{
     
     
     Serial.begin(9600);

     gpioInit();
     digitalWrite(8,HIGH);
     interruptInit();

     //Initialize accelerometer
     lis.begin(0x18);
     lis.setRange(LIS3DH_RANGE_2_G);
     lis.setDataRate(LIS3DH_DATARATE_50_HZ);

     //Initialize mp3 player
     speakerInit();

    // Initialize display
    displayInit();

     sei();
     
}

void loop()
{
    
     
         
     
    

     Serial.print("State: ");
     Serial.println(currentState);
     
     switch (currentState)
     {
     case prestart:
          displayStartingMessage();

          set_sleep_mode(SLEEP_MODE_PWR_DOWN);
           sleep_enable();
           sleep_cpu();
          // Wakes here after start button ISR
          displayScore(score);
          currentState = rand() % 4;
          break;  // was missing before - caused fall-through into mash

     case mash:
          generateSound(mash);
          delay(delayms);

          if (mashbuttonCount >= REQUIRED_COUNT)
          {
               score++;
               displayScore(score);
               if (score >= 99)
               {
                    currentState = win;
               }
               else
               {
                    currentState = rand() % 4;
                    updateDelay();
               }
          }
          else
          {
               currentState = lose;
          }
          mashbuttonCount = 0;
          break;

     case shake:
          generateSound(shake);
          delay(delayms);
          lis.getEvent();
          lis.read();
          if (abs(lis.z) >= SHAKE_THRESH || abs(lis.y) >= SHAKE_THRESH || abs(lis.x) >= SHAKE_THRESH)
          {
               score++;
               displayScore(score);
               if (score >= 99)
               {
                    currentState = win;
               }
               else
               {
                    currentState = rand() % 4;
                    updateDelay();
               }
          }
          else
          {
               currentState = lose;
          }
          break;

     case hide:
          generateSound(hide);
          delay(delayms);
          digitalWrite(indicatorLEDPin, HIGH);

          if (readPhotoRes())
          {
               score++;
               displayScore(score);
               if (score >= 99)
               {
                    currentState = win;
               }
               else
               {
                    currentState = rand() % 4;
                    updateDelay();
               }
          }
          else
          {
               currentState = lose;
          }
          digitalWrite(indicatorLEDPin, LOW);
          break;

     case joke:
          generateSound(joke);
          delay(delayms);

          if (mashbuttonCount != 0)
          {
               currentState = lose;
               break;
          }
          lis.read();
          if (abs(lis.z) >= SHAKE_THRESH || abs(lis.y) >= SHAKE_THRESH || abs(lis.x) >= SHAKE_THRESH)
          {
               currentState = lose;
               break;
          }

          currentState = rand() % 4;
          break;

     case win:
          displayMessage("You Win!", score);
          generateSound(win);
          while (1) { }
          break;

     case lose:
          displayMessage("You Lose!", score);
          generateSound(lose);
          while (1) { }
          break;
     }
}