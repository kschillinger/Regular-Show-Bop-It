

/*
Main File for bop it game.
Last Updated: 3/7/2026
Todo

     


Open questions 
Debouncing? hardware debounce 


*/
#include <Arduino.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/sleep.h>
#include <avr/power.h>
#include<avr/interrupt.h>

extern "C"
{
     #include "init.h"
}
//Globals
#define REQUIRED_COUNT 10 // User must mash buttons 10 times for success
#define REQUIRED_ACC 15 //  Arbitrary placeholder User must shake with certain acceration for success
enum state : uint8_t{mash, shake, hide,joke,prestart,win,lose};
uint8_t currentState;
uint16_t lbuttonCount=0,rbuttonCount=0; 
uint8_t score = 0x00;      // Initialize score to 0
uint8_t successful = 0x01; // Initialize to successful user input dont know if this is needed
uint8_t delayms=5000; //start at 5s
//ISRs
void leftButtonISR()
{
     lbuttonCount++;
}
void rightButtonISR()
{
     rbuttonCount++;
}
void startButtonISR()
{
     sleep_disable(); //restart execution
}

/*
     Output sound from memory based on the current state 
     @param void
     @return void
*/
void generateSound() //may be removed, df player needs uart function can just be called in main 
{

     

}
void readInputUpdate()
{

}
void speakerOutput(const char *phrase);
   



// Display
void display_winning_screen(void);
void display_losing_screen(void);

// Hardware reads

//accelerometer info is i2c or spi
bool readAccelerometer(void);
bool  readPhotoRes(void);
void screenUpdate();


int main(void)
{
     
     while(1)
     {
     
          switch(currentState)
          {
               case(prestart):
               
                    //put cpu into low power mode
                    //only interrupts and timer2 active
                    set_sleep_mode(SLEEP_MODE_PWR_DOWN);
                    sleep_enable();
                    sleep_cpu(); 
                    //start button triggers interrupt and takes out of sleep state
                    currentState = rand() % (4); //start the game
               
               
               //since interrupts are needed to read the button clicks no Buttone read function is needed
               case(mash): 
                    generateSound();
                    //buttons read w/ interrupts
                    delay(delayms);
                    //check success
                    if(lbuttonCount + rbuttonCount >= REQUIRED_COUNT) 
                    {
                         score ++;
                         screenUpdate();
                         currentState = rand() % (4); //randomly select next action 0-3 
                         if(score >= 99) // winning score overwrite
                         {
                              currentState = win;
                         }
                    }
                    else
                    {
                         successful = 0; //delete?
                         currentState = lose; //transition to lose state
                    }
                    //reset click counts
                    lbuttonCount=0;
                    rbuttonCount=0;
                    break;
                    
               case(shake):

                    generateSound();
                    delay(delayms);
                    //read accelerometer
                    if(true) //should be if acceleration is above threshold
                    {
                         score++;
                         screenUpdate();
                         currentState = rand() % (4); //randomly select next action 0-3 
                         if(score >= 99) // winning score overwrite
                         {
                              currentState = win;
                         }
                    }
                    else
                    {
                         successful = 0; //delete?
                         currentState = lose; //transition to lose state
                    }
                    
               
                    break;
               case(hide):

                    generateSound();
                    delay(delayms);
                    if(true) //should be volatage thresh check 
                    {
                         score++;
                         screenUpdate();
                         currentState = rand() % (4); //randomly select next action 0-3 
                         if(score >= 99) // winning score overwrite
                         {
                              currentState = win;
                         }
                    }
                    else
                    {
                         successful = 0; //delete?
                         currentState = lose; //transition to lose state
                    }

                    break;

              
               case(joke):
                    rand() % 3;//assuming we have 2 jokes
                    //output joke 
                    delay(delayms);
               
                    
                    break;

               case(win):
                    generateSound();
                    while(1)//continuously show victory until restart or shut off
                    {
                         display_winning_screen();
                    }
                    break;
               case(lose):
                    while(1)
                    {
                         display_losing_screen();
                    }

                    break;
               


          } 
     }
}
 
