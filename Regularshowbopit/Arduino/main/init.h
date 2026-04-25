#ifndef INIT_H
#define INIT_H

void gpioInit();
void interruptInit();


void mashButtonISR(void);
void startButtonISR(void);

#endif