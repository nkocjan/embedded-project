#ifndef KEYBOARD_H
#define KEYBOARD_H
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "app.h"
#include "asciiLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

void initializeKeyboard() {
  // Definicja wierszy i kolumn
  int ROWS = (1 << 3) | (1 << 2) | (1 << 1) |
             (1 << 0); // P0.21, P0.19, P0.17, P0.16
  int COLS = (1 << 24) | (1 << 22) | (1 << 20) |
             (1 << 18); // P0.22, P0.20, P0.18, P0.15

  // **WIERSZE (WEJŚCIA)** (P0.21, P0.19, P0.17, P0.16)
  LPC_PINCON->PINSEL0 &= ~((3UL << 6) | (3UL << 4) | (3UL << 2) |
                           3UL); // GPIO dla P0.21, P0.19, P0.17, P0.16
  LPC_GPIO0->FIODIR &= ~ROWS;         // jako wejścia
  LPC_PINCON->PINMODE0 |=
      ((2 << 6) | (2 << 4) | (2 << 2) |
       (2 << 0)); // Tryb high-impedance (brak pull-up/pull-down)

  // **KONFIGURACJA PRZERWAŃ DLA WEJŚĆ**
  LPC_GPIOINT->IO0IntEnF |=
      ROWS; // przerwania dla zbocza opadającego (naciśnięcie klawisza)
  LPC_GPIOINT->IO0IntEnR |=
      ROWS; // przerwania dla zbocza narastającego (puszczenie klawisza)

  // **KOLUMNY (WYJŚCIA - OPEN-DRAIN)** (P0.22, P0.20, P0.18, P0.15)
  LPC_PINCON->PINSEL1 &= ~((3 << 16) | (3 << 12) | (3 << 8)); // GPIO dla P0.22, P0.20, P0.18, P0.15
	LPC_PINCON->PINSEL0 &= ~((3 << 4));
  LPC_GPIO0->FIODIR |= COLS;           // jako wyjścia
  LPC_PINCON->PINMODE_OD1 |=
      ((1 << 24) | (1 << 22) | (1 << 20) | (1 << 18)); // Open-drain dla wyjść

  // Przerwania GPIO w NVIC
  NVIC_EnableIRQ(EINT3_IRQn); // Przerwania GPIO są obsługiwane przez EINT3
}

void getPressedKey(char *str) {
  *str = '\0';

  for (int i = 0; i < 4; i++) {
    for (int j = 0; j < 4; j++) {
      if (keyState[i][j]) {
        char c = keypadChars[i][j];

        int len = strlen(str);

        str[len] = c;
        str[len + 1] = '\0';
      }
    }
  }
}



#endif
