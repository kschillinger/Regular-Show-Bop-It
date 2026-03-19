

/*
Main File for bop it game.
Last Updated: 3/7/2026
Todo
Sound selection logic -Done (untested)
Sound output - Done (untested)
Screen stuff - Done (untested)
accelerometer read - done (untested)
photosensor read 

accelerometer and photosensor read in the joke state


Open questions
Do we need to disable interrupts when reading the buttons?

Debouncing 
     -hardware debounce

need a good way to show user where the light sensor is
    -led indicator that shuts off when the cover is successful
   
df player memory addressing
     -landis got this squared away right? 
when the user restarts mid game the state case will finish executing before restarting, need a way to preemmpt and force execution to restart
    -solution is to send high to reset pin which resets cpu 
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
#include <speakerConfig.hpp>
#include <displayConfig.hpp>


// Screen driver header
#include <SPI.h>
#include <U8g2lib.h>

extern "C"
{
#include "init.h"
}
// Globals
#define REQUIRED_COUNT 10 // User must mash buttons 10 times for success
#define REQUIRED_ACC 15   //  Arbitrary placeholder User must shake with certain acceration for success
#define GLOBAL_DEL 5000   // start at 5s
#define SHAKE_THRESH 16384 // currently at +- 4G 1G is 8192 setting threshold at 2G

// Pin definitions
#define photoPin 2
#define leftButtonPin 3
#define rightButtonPin 4
#define startButtonPin 5
#define indicatorLEDPin 11


enum state : uint8_t
{
     mash,
     shake,
     hide,
     joke,
     prestart,
     win,
     lose
};
uint8_t currentState = prestart; // start in sleep mode
uint16_t lbuttonCount = 0, rbuttonCount = 0;
uint8_t score = 0x00;                        // Initialize score to 0
uint8_t successful = 0x01;                   // Initialize to successful user input dont know if this is needed
uint8_t delayms = GLOBAL_DEL;                // initilaize delay value to max delay
uint8_t increment = floor(GLOBAL_DEL / 100); // get delay increment 5s del div=50ms
Adafruit_LIS3DH lis = Adafruit_LIS3DH();
LiquidCrystal_PCF8574 lcd(0x27);

// ISRs
void leftButtonISR()
{
     lbuttonCount++;
     //reset flag
}
void rightButtonISR()
{
     rbuttonCount++;
     //reset flag
}
void startButtonISR()
{


     if (score > 0) // user restarting mid game or at end of game
     {
        
          //send high to reset pin to restart execution

          

     }
     else
     {
          sleep_disable(); // restart execution
          
          //for now we comment it out until further testing is done
          //generateSound(prestart);  // start the game with a countdown sound
     }
     //reset flag
}





// Hardware reads


bool readPhotoRes(void)
{
     if (digitalRead(photoPin) == HIGH)
     {
          return true;
     }
     else
     {
          return false;
     }
}

// helpers
void updateDelay()
{
     if (delayms - increment <= 0) // should never happen
     {
          delayms = 200;
     }
     else
     {
          delayms -= increment;
     }
}

int main(void)
{
     interruptInit();
     gpioInit();
    
 
     // initialize accelerometer
     lis.begin(0x18);                // default I2C address
     lis.setRange(LIS3DH_RANGE_2_G); // set to +- 2g acceleration range
     lis.setDataRate(LIS3DH_DATARATE_50_HZ);

     //initialize the mp3 player
     speakerInit();
     displayInit();
     //enable interrupts 
    sei(); //master interrupt enable 
     while (1)
     {

          switch (currentState)
          {
          case (prestart):
               displayStartingMessage();

               // put cpu into low power mode
               // only interrupts and timer2 active
               set_sleep_mode(SLEEP_MODE_PWR_DOWN);
               sleep_enable();
               sleep_cpu();
               // start button triggers interrupt and takes out of sleep state
               displayScore(score);
               currentState = rand() % (4); // start the game

          // since interrupts are needed to read the button clicks no Buttone read function is needed
          case (mash):
               generateSound(mash);
               // buttons read w/ interrupts
               delay(delayms);
               // check success

               if (lbuttonCount + rbuttonCount >= REQUIRED_COUNT)
               {
                    score++;
                    displayScore(score);

                    if (score >= 99) // winning score
                    {
                         currentState = win;
                    }
                    else
                    {
                         currentState = rand() % (4); // randomly select next action 0-3
                         updateDelay();
                    }
               }
               else
               {
                    successful = 0;      // delete?
                    currentState = lose; // transition to lose state
               }
               // reset click counts
               lbuttonCount = 0;
               rbuttonCount = 0;
               break;

          case (shake):

               generateSound(shake);
               delay(delayms);
               // read accelerometer
               lis.read(); // populates the xyz vals
               //magnitude of at least one of the axes must be above threshold
               if (abs(lis.z) >= SHAKE_THRESH || abs(lis.y) >= SHAKE_THRESH || abs(lis.x) >= SHAKE_THRESH)
               {
                    score++;
                    displayScore(score);
                    if (score >= 99) // winning score
                    {
                         currentState = win;
                    }
                    else
                    {
                         currentState = rand() % (4); // randomly select next action 0-3
                         updateDelay();
                    }
               }
               else
               {
                    successful = 0;      // delete?
                    currentState = lose; // transition to lose state
               }

               break;
          case (hide):

               generateSound(hide);
               delay(delayms);
               digitalWrite(indicatorLEDPin, HIGH); // turn on indicator led to show user where the light sensor is

               if (true) // should be volatage thresh check
               {
                    score++;
                    displayScore(score);

                    if (score >= 99) // winning score
                    {
                         currentState = win;
                    }
                    else
                    {
                         currentState = rand() % (4); // randomly select next action 0-3
                         updateDelay();
                    }
               }
               else
               {
                    successful = 0;      // delete?
                    currentState = lose; // transition to lose state
               }
               digitalWrite(indicatorLEDPin, LOW); // turn off indicator led
               break;

          case (joke):

               generateSound(joke);
               delay(delayms);
               
               
               //check all inputs 
               if (lbuttonCount + rbuttonCount != 0)
               {
                    successful = 0;      // delete?
                    currentState = lose; // transition to lose state
                    break;
               }
               lis.read(); // populates the xyz vals
               //magnitude of at least one of the axes must be above threshold
               if (abs(lis.z) >= SHAKE_THRESH || abs(lis.y) >= SHAKE_THRESH || abs(lis.x) >= SHAKE_THRESH)
               {
                    successful = 0;      // delete?
                    currentState = lose; // transition to lose state
                    break;
                    
               }
               //volatage threshold check for photosensor

               //no inputs detected
               currentState = rand() % (4); // randomly select next action 0-3
               
               break;

          case (win):
               displayMessage("You Win!", score);
               generateSound(win);
               while (1) // continuously show victory until restart or shut off
               {
               }
               break;
          case (lose):

               displayMessage("You Lose!", score);
               generateSound(lose);
               while (1)
               {
               }

               break;
          }
     }
}
