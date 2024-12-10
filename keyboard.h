#ifndef KEYBOARD_H
#define KEYBOARD_H
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
#include <stdbool.h>
#include "app.h"






void initializeKeyboard() {
  // Definicja wierszy i kolumn
  int ROWS = (1 << 21) | (1 << 19) | (1 << 17) |
             (1 << 16); // P0.21, P0.19, P0.17, P0.16
  int COLS = (1 << 22) | (1 << 20) | (1 << 18) |
             (1 << 15); // P0.22, P0.20, P0.18, P0.15

  // **WIERSZE (WEJŚCIA)** (P0.21, P0.19, P0.17, P0.16)
  LPC_PINCON->PINSEL1 &= ~((3 << 10) | (3 << 6) | (3 << 2) |
                           (3 << 0)); // GPIO dla P0.21, P0.19, P0.17, P0.16
  LPC_GPIO0->FIODIR &= ~ROWS;         // jako wejścia
  LPC_PINCON->PINMODE1 |=
      ((2 << 10) | (2 << 6) | (2 << 2) |
       (2 << 0)); // Tryb high-impedance (brak pull-up/pull-down)

  // **KONFIGURACJA PRZERWAŃ DLA WEJŚĆ**
  LPC_GPIOINT->IO0IntEnF |=
      ROWS; // przerwania dla zbocza opadającego (naciśnięcie klawisza)
  LPC_GPIOINT->IO0IntEnR |=
      ROWS; // przerwania dla zbocza narastającego (puszczenie klawisza)

  // **KOLUMNY (WYJŚCIA - OPEN-DRAIN)** (P0.22, P0.20, P0.18, P0.15)
  LPC_PINCON->PINSEL1 &= ~((3 << 12) | (3 << 8) | (3 << 4) |
                           (3 << 30)); // GPIO dla P0.22, P0.20, P0.18, P0.15
  LPC_GPIO0->FIODIR |= COLS;           // jako wyjścia
  LPC_PINCON->PINMODE_OD0 |=
      ((1 << 22) | (1 << 20) | (1 << 18) | (1 << 15)); // Open-drain dla wyjść

  // Przerwania GPIO w NVIC
  NVIC_EnableIRQ(EINT3_IRQn); // Przerwania GPIO są obsługiwane przez EINT3
}

#endif
