#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "app.h"
#include "asciiLib.h"
#include "uart.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void initializeKeyboard() {
  // Definicja wierszy i kolumn
  int ROWS = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);     // P0
  int COLS = (1 << 21) | (1 << 20) | (1 << 19) | (1 << 18); // P1

  // Konfiguracja wierszy (P0.0 - P0.3)
  LPC_PINCON->PINSEL0 &= ~((3UL << 6) | (3UL << 4) | (3UL << 2) | 3UL);
  LPC_GPIO0->FIODIR &= ~ROWS; // Wiersze jako wejścia
  LPC_PINCON->PINMODE0 |=
      ((2 << 6) | (2 << 4) | (2 << 2) | (2 << 0)); // Brak pull-up/pull-down

  // Konfiguracja przerwań dla wejść
  LPC_GPIOINT->IO0IntEnF |= ROWS; // Zbocze opadające (naciśnięcie klawisza)

  // Konfiguracja kolumn (P1.18 - P1.21)
  LPC_PINCON->PINSEL3 &=
      ~((3 << 6) | (3 << 4) | (3 << 2) | 3); // GPIO dla P1.18 - P1.21
  LPC_GPIO1->FIODIR |= COLS;                 // Kolumny jako wyjścia
  LPC_PINCON->PINMODE_OD1 |= COLS;           // Open-drain dla wyjść

  // Przerwania GPIO w NVIC
  NVIC_EnableIRQ(EINT3_IRQn); // Przerwania GPIO obsługiwane przez EINT3
}

void EINT3_IRQHandler() {
  int COLS = (1 << 21) | (1 << 20) | (1 << 19) | (1 << 18); // P1.21 - P1.18
  int ROWS =
      (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0); // P0.0, P0.1, P0.2, P0.3

  // Wyłączenie przerwań od klawiatury
  LPC_GPIOINT->IO0IntEnF &= ~ROWS;
  LPC_GPIOINT->IO0IntClr = ROWS;

  wasInterupted = true;
  // Ponowne włączenie przerwań od klawiatury

}

void readKeyboard() {
  int ROWS = (1 << 3) | (1 << 2) | (1 << 1) | (1 << 0);     // P0.0 - P0.3
  int COLS = (1 << 21) | (1 << 20) | (1 << 19) | (1 << 18); // P1.21 - P1.18

  int row, col;
  for (row = 0; row < 4; row++) {
    if (((LPC_GPIO0->FIOPIN >> row) & 1) == 0)
      break;
  }
	    LPC_GPIO1->FIOSET |= COLS;
  for (col = 0; col < 4; col++) {

    LPC_GPIO1->FIOCLR = (1 << (col + 18));
    uint32_t now = msTicks;
    while (msTicks - now < 5);

    if (((LPC_GPIO0->FIOPIN >> row) & 1) == 0)
      break;
  }

  LPC_GPIO1->FIOCLR |= COLS;

  char stroke = keypadChars[row][col];

  char txt[] = {stroke, '\n', '\0'};
  UART_SEND(txt, 3);

  // Ponowne włączenie przerwań od klawiatury
  LPC_GPIOINT->IO0IntEnF |= ROWS;
  wasInterupted = false;
}

#endif
