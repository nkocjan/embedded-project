#ifndef RTC_H
#define RTC_H
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "TP_Open1768.h"
#include "asciiLib.h"
#include <Board_Buttons.h>
#include <Board_LED.h>
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "uart.h"
#include "app.h"
void initializeRTC() {
  LPC_SC->PCONP |= (1 << 9);
  LPC_RTC->CCR = 1;
  LPC_RTC->ILR = 0;
  LPC_RTC->CIIR = 1;

  NVIC_EnableIRQ(RTC_IRQn);
}


#endif
