#include <Arduino.h>

/*
Main File for bop it game.
Last Updated: 3/7/2026
Todos everything

*/

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include <stdbool.h>
 
#define REQUIRED_COUNT 10 // User must mash buttons 10 times for success
#define REQUIRED_ACC 15 //  Arbitrary placeholder User must shake with certain acceration for success

enum state : uint8_t{mash, shake, hide,joke,win,lose};
uint8_t currentState;
 uint16_t button1Count=0,button2Count=0; 

uint8_t score = 0x00;      // Initialize score to 0
uint8_t successful = 0x01; // Initialize to successful user input dont know if this is needed
void uartInit(uint32_t baud)
{

}
/*
     Choose prompt and generate sound 
     @param void
     @return void
*/
void chooseAndPromptAction() 
{
     //Select sound based on the random int and output
 	 currentState = rand() % (4);// random int  =  min + rand mod (max-min+1) Update current action 
     uint8_t prompt = (uint8_t)0x01;
     generateSound();    // Prompt user based on current action
     prompt = (uint8_t)0; //reset prompt
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
const char *get_joke(int index);

// Display
void display_winning_screen(void);
void display_losing_screen(void);

// Hardware reads

//accelerometer info is i2c or spi
bool readAccelerometer(void);
bool  readPhotoRes(void);



int main(void)
{
     while(1)
     {
         switch(currentState)
     {
          //since interrupts are needed to read the button clicks no Buttone read function is needed
          case(mash): 
             
               //buttons read w/ interrups
               //check success
               if(button1Count+button2Count >= REQUIRED_COUNT) 
               {
                    score ++;
                    currentState = rand() % (4); //randomly select next action 0-3 
               }
               else
               {
                    successful = 0; //delete?
                    currentState = rand() % (4); //transition to lose state
               }
               //check score
               //state transition
               
               break;
          case(shake):
               //read accelerometer
 
          
               break;
          case(hide):

               break;

          case(win):
               break;

          case(lose):

               break;
          case(joke):


     } 
     }
}
 
