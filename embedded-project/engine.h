#ifndef ENGINE_H
#define ENGINE_H


#include "app.h"
#include "uart.h"
#include <cstdint>



void initiEngine(){
	int enginePin = 1 << 11;
	LPC_PINCON->PINSEL0 &= ~(3UL << 22);
	LPC_GPIO0->FIODIR |= enginePin;
}

void startEngine(){

	LPC_GPIO0->FIOSET = 1 << 11; 
	
}

void stopEngine(){
	LPC_GPIO0->FIOCLR = 1 << 11;
}


#endif
