/*
Main File for bop it game.
Last Updated: 3/24/2026
Rewritten for Arduino IDE using setup()/loop()

todo add loop for accelerometer polling
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

extern "C" {
#include "init.h"
}

// Globals
#define REQUIRED_COUNT 5
#define GLOBAL_DEL 5000
#define SHAKE_THRESH 16384

// Pin definitions
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
#define DEBOUNCETIME .005
// State variables
uint8_t currentState = prestart;
volatile uint16_t mashbuttonCount = 0;  // volatile - modified in ISR
uint8_t score = 0x00;
uint16_t delayms = GLOBAL_DEL;
uint8_t increment = GLOBAL_DEL / 100;
sensors_event_t event;
volatile unsigned long lastPressTimeMash = 0;
volatile unsigned long lastPressTimeReset = 0;

// Peripherals
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
Adafruit_LiquidCrystal lcd(0x27);

// ISRs - interrupts are already disabled when an ISR runs,
// so no need for cli()/sei() inside them
void mashButtonISR() {
  if(currentState ==mash || currentState == joke) //we only want to read buttone presses in the mash and joke state
  {
    Serial.println("mashisr"); 

    unsigned long now = millis();
    if (now - lastPressTimeMash > DEBOUNCETIME) {  // 50 ms debounce window
      mashbuttonCount++;
      lastPressTimeMash = now;
    }
  }

}

void startButtonISR() {
     Serial.println("startisr");
  unsigned long now = millis();
 // detachInterrupt(digitalPinToInterrupt(3));
  if (now - lastPressTimeReset > DEBOUNCETIME) {
    lastPressTimeReset = now;
    if (score > 0) {
      digitalWrite(resetSignalPin, LOW);
    } else {
      sleep_disable();
    }
  }
}

// Hardware reads
bool readPhotoRes(void) {
  return (digitalRead(photoPin) == HIGH);
}

// Helpers
void updateDelay() {
  if (delayms <= increment) {
    delayms = 200;
  } else {
    delayms -= increment;
  }
}

void setup() {


  Serial.begin(9600);

  gpioInit();

  digitalWrite(8, HIGH);

  interruptInit();

  //Initialize accelerometer
  lis.begin(0x18);

  lis.setRange(LIS3DH_RANGE_2_G);

  lis.setDataRate(LIS3DH_DATARATE_50_HZ);

  //Initialize mp3 player
  //speakerInit();

  // Initialize display
  displayInit();
     srand(analogRead(A3));
  sei();
}

void loop() {


     Serial.print("Score =");
     Serial.println(score);
     //Serial.println(currentState);
     delay(1000);
  switch (currentState) {
    case prestart:
      displayStartingMessage();


       set_sleep_mode(SLEEP_MODE_PWR_DOWN);
       sleep_enable();
       sleep_cpu();
      // Wakes here after start button ISR
     // displayScore(score);
      Serial.println("prestart");
     // currentState = rand() % 4;
      currentState = hide;
      break;  // was missing before - caused fall-through into mash

    // case mash:
    //       Serial.println("mash");
    //   //generateSound(mash);
    //   delay(delayms);

    //   if (mashbuttonCount >= REQUIRED_COUNT) {
    //     score++;
    //     //displayScore(score);
    //     if (score >= 99) {
    //       currentState = win;
    //     } else {
          
    //        //currentState = rand() % 4;
    //       updateDelay();
    //     }
    //   } else {
    //     currentState = lose;
    //   }
    //   Serial.print(mashbuttonCount);
    //   mashbuttonCount = 0;
    //   break;

    // case shake:
    //  Serial.println("SHAKE");
    //   //generateSound(shake);
    //   float x=0,y=0,z=0;

    //   for(int i=0;i<10;i++) //sample accelerometer 10x
    //   {
        
    //     lis.read();
    //     x+= abs(lis.x);
    //     y+= abs(lis.y);
    //     z+= abs(lis.z);
    //     delay(delayms/10);
    //   }
    //   //average reading
    //   x=x/10;
    //   y=y/10;
    //   z=z/10;
      
    //  // lis.getEvent(&event);

    //   if (x >= SHAKE_THRESH || y >= SHAKE_THRESH || x >= SHAKE_THRESH) {
    //     score++;
    //     displayScore(score);
    //     if (score >= 99) {
    //       currentState = win;
    //     } else {
    //       currentState = shake;
    //       updateDelay();
    //     }
    //   } else {
    //     currentState = lose;
    //   }
    //   break;

    case hide:
    Serial.println("hide");
      //generateSound(hide);
        digitalWrite(indicatorLEDPin, HIGH);
      

  
      //Serial.print("hide");
      bool covered=false;
      for(int i=0;i<10;i++)
      {
        if(readPhotoRes())
          covered=true;
        delay(delayms/10);
      }
      if (covered) 
      {
        score++;
        displayScore(score);
        if (score >= 99) {
          currentState = win;
        } else {
          // currentState = rand() % 4;
          currentState =hide;
          updateDelay();
        }
      } 
      else 
      {
        currentState = lose;
      }
      digitalWrite(indicatorLEDPin, LOW);
      break;

    // case joke:
    // Serial.println("joke");
    //   //generateSound(joke);
    //   delay(delayms);

    //   if (mashbuttonCount != 0) {
    //     currentState = lose;
    //     break;
    //   }
    //   lis.read();
    //   if (abs(lis.z) >= SHAKE_THRESH || abs(lis.y) >= SHAKE_THRESH || abs(lis.x) >= SHAKE_THRESH) {
    //     currentState = lose;
    //     break;
    //   }

    //   currentState = rand() % 4;
    //   break;

    // case win:
    // Serial.println("win");
    //   displayMessage("You Win!", score);
    //   //generateSound(win);
    //   while (1) {}  //sticking here may be a problem
    //   break;

    case lose:
    Serial.print("lose");
      displayMessage("You Lose!", score);
      //generateSound(lose);
      Serial.print("loser landis");
      while (1);
      break;
  }
}