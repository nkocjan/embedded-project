#ifndef APP_H
#define APP_H

#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "asciiLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

extern volatile int minutes;
extern volatile int hours;
extern volatile int months;
extern volatile int years;
extern volatile char date[30];
extern int x_wsk;
extern int y_wsk;
extern bool keyState[4][4];
extern volatile uint32_t msTicks;
extern volatile bool wasInterupted;
extern volatile uint32_t lastScreenDebounce;
extern const uint32_t debounceDelay;
extern const char keypadChars[4][4];
extern char keyPresses[16];
extern char stroke;

#endif
