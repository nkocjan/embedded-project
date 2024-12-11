#ifndef HANDLERS_H
#define HANDLERS_H

#include "app.h"
#include "uart.h"

void RTC_IRQHandler() {
  static int tikTak = 1;

  if (tikTak) {
    UART_SEND("TIK ", 4);
    tikTak = 0;
  } else {
    UART_SEND("TAK ", 4);
    tikTak = 1;
  }

  LPC_RTC->ILR = 1;
}

void SysTick_Handler(void) { msTicks++; }

void TIMER0_IRQHandler() {
  UART_SEND("PING\n", 5);
  LPC_TIM0->IR = 1;
}

void EINT0_IRQHandler() {
  static int led = 1;
  if (LPC_SC->EXTINT & 1) {
    led = (led == 1 ? 0 : 1);
    UART_SEND("Click\n", 6);
  }
  LPC_SC->EXTINT = 1;
}

void EINT3_IRQHandler(void) {
  // Definicja wierszy i kolumn
  int ROWS = (1 << 21) | (1 << 19) | (1 << 17) |
             (1 << 16); // P0.21, P0.19, P0.17, P0.16
  int COLS = (1 << 22) | (1 << 20) | (1 << 18) |
             (1 << 15); // P0.22, P0.20, P0.18, P0.15

  int rows[4] = {21, 19, 17, 16};
  int cols[4] = {22, 20, 18, 15};
  // Sprawdzanie przerwań dla wierszy (przy zboczu opadającym lub narastającym)
  for (int row = 0; row < 4; row++) {
    int rowPin =
        rows[row]; // Obliczanie pinu wiersza: P0.21, P0.19, P0.17, P0.16

    if (LPC_GPIOINT->IO0IntStatF &
        (1 << rowPin)) { // Zbocze opadające (naciśnięcie klawisza)
      // Obsługa naciśnięcia klawisza
      for (int col = 0; col < 4; col++) {
        int colPin =
            cols[col]; // Obliczanie pinu kolumny: P0.22, P0.20, P0.18, P0.15

        if (!(LPC_GPIO0->FIOPIN & (1 << colPin))) { // Jeśli stan kolumny niski
          keyState[row][col] = true;                // Klawisz wciśnięty
        }
      }
    }

    if (LPC_GPIOINT->IO0IntStatR &
        (1 << rowPin)) { // Zbocze narastające (puszczenie klawisza)
      // Obsługa puszczenia klawisza
      for (int col = 0; col < 4; col++) {
        int colPin =
            22 -
            (col * 2); // Obliczanie pinu kolumny: P0.22, P0.20, P0.18, P0.15

        if (LPC_GPIO0->FIOPIN & (1 << colPin)) { // Jeśli stan kolumny wysoki
          keyState[row][col] = false;            // Klawisz puszczony
        }
      }
    }
  }

  // Czyszczenie flag przerwania dla wierszy
  LPC_GPIOINT->IO0IntClr =
      ROWS; // Czyszczenie wszystkich flag przerwania dla portu 0
}
#endif
