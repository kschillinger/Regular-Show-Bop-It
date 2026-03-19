#ifndef INIT_H
#define INIT_H

void gpioInit();
void interruptInit();
void timerInit(); //i cant remember why i included --> nice

// Forward declarations of ISRs defined in main.cpp
void leftButtonISR(void);
void rightButtonISR(void);
void startButtonISR(void);



#endif