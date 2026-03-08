

/*
Main File for bop it game.
Last Updated: 3/7/2026
Todo
Sound selection logic
Sound output
Screen stuff
accelerometer read
photosensor read

     


Open questions 
Debouncing? hardware debounce 
screen refresh?
need a good way to show user where the light sensor is
    blink led?
    modify enclosure to make it obvious?
df player memory addressing
when the user restarts mid game the state case will finish executing before restarting, need a way to preemmpt and force execution to restart
    -is it possible to send a reset signal through hardware?
    problem with that is that a second click would be needed to take the proc out of sleep mode 
    don't think this is the way to go
    does the user even need to be able to restart mid game?
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
#define GLOBAL_DEL 5000 //start at 5s
enum state : uint8_t{mash, shake, hide,joke,prestart,win,lose};
uint8_t currentState=prestart; //start in sleep mode
uint16_t lbuttonCount=0,rbuttonCount=0; 
uint8_t score = 0x00;      // Initialize score to 0
uint8_t successful = 0x01; // Initialize to successful user input dont know if this is needed
uint8_t delayms=GLOBAL_DEL; //initilaize delay value to max delay 
uint8_t increment = floor(GLOBAL_DEL/100); //get delay increment 5s del div=50ms

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
     
     if(score>0) //user restarting mid game or at end of game 
     {
       //reset all game vars
       score=0;
       lbuttonCount=0;
       rbuttonCount=0;
       delayms=GLOBAL_DEL;
       currentState = rand() % (4); 
       //preempt and restart exection somehow
     }
     else
     {
          sleep_disable(); //restart execution
    
     }
}

/*
     Output sound from memory based on the current state 
     @param void
     @return void
*/
void generateSound() //may be removed, df player needs uart function can just be called in main 
{


     

}
void speakerOutput(const char *phrase);
   



// Display
void display_winning_screen(void);
void display_losing_screen(void);
void screenUpdate();
// Hardware reads

//accelerometer info is i2c or spi
bool readAccelerometer(void);
bool  readPhotoRes(void);

//helpers
void updateDelay()
{
     if(delayms-increment<=0) //should never happen
     {
          delayms=200;
     }
     else
     {
          delayms-=increment;
     }

}


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
                        
                         if(score >= 99) // winning score 
                         {
                              currentState = win;
                         }
                         else
                         {
                             currentState = rand() % (4); //randomly select next action 0-3 
                             updateDelay();
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
                         if(score >= 99) // winning score 
                         {
                              currentState = win;
                         }
                         else
                         {
                             currentState = rand() % (4); //randomly select next action 0-3 
                             updateDelay();
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

                         if(score >= 99) // winning score 
                         {
                              currentState = win;
                         }
                         else
                         {
                             currentState = rand() % (4); //randomly select next action 0-3 
                             updateDelay();
                         }
                    }
                    else
                    {
                         successful = 0; //delete?
                         currentState = lose; //transition to lose state
                    }

                    break;

              
               case(joke):

                   
                    generateSound();
                    delay(delayms);
                    if(true) //no input im thinking they get no points for ignoring and the delay doesn't change
                    {
                         currentState = rand() % (4); //transition
                    }
                    else //cooked
                    {
                         if(lbuttonCount + rbuttonCount !=0)
                         {
                              successful = 0; //delete?
                              currentState = lose; //transition to lose state
                              break;
                         }
                         //if accelerometer

                         //if light sense 
                         
                    }
                    
                    break;

               case(win):
                    generateSound();
                    while(1)//continuously show victory until restart or shut off
                    {
                         display_winning_screen();
                    }
                    break;
               case(lose):
               
                    generateSound();                
                    while(1)
                    {
                         display_losing_screen();
                    }

                    break;
               


          } 
     }
}
 
