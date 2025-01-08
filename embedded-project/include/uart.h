#ifndef UART_H
#define UART_H
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "asciiLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdbool.h>


extern ARM_DRIVER_USART Driver_USART1;

static ARM_DRIVER_USART *USARTdrv = &Driver_USART1;

void UART_SEND(const char *wsk, int len) { USARTdrv->Send(wsk, len);}; 

void initializeUART2(void){
      USARTdrv->Initialize(NULL);
  USARTdrv->PowerControl(ARM_POWER_FULL);
  USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 |
                        ARM_USART_PARITY_NONE | ARM_USART_STOP_BITS_1 |
                        ARM_USART_FLOW_CONTROL_NONE,
                    19200);
  USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
  USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
}

void myUART_Thread(const void *args) {
  char cmd;
  USARTdrv->Send("\nPress Enter to receive a message", 34);
  while (1) {
    while (USARTdrv->GetStatus().tx_busy) {
    }
    USARTdrv->Receive(&cmd, 1);
    while (USARTdrv->GetStatus().rx_busy) {
    }

    if (cmd == 13) {
      USARTdrv->Send("\nHello World!", 12);
    }
  }
}

#endif
