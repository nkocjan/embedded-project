#ifndef APP_H
#define APP_H

#include <stdbool.h>
#include <stdint.h>
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "TP_Open1768.h"
#include "asciiLib.h"
#include <Board_Buttons.h>
#include <Board_LED.h>
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
// Podstawowy punkt, używany w LCD i touchpanelu.
typedef struct {
    int x;
    int y;
} Point;


int x_wsk;
int y_wsk;
extern bool keyState[4][4];
volatile uint32_t msTicks = 0;
volatile uint32_t lastScreenDebounce = 0;
const uint32_t debounceDelay = 50;


#endif
