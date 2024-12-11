#ifndef HANDLERS_H
#define HANDLERS_H

#include "app.h"
#include "uart.h"

volatile uint32_t msTicks = 0;

void SysTick_Handler(void) {
    msTicks++;
}

/* Wait function using msTicks */
void wait(uint32_t waitTimeMs) {
    msTicks = 0;
    while (msTicks < waitTimeMs) {}
}

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
   int ROWS = (1 << 3) | (1 << 2) | (1 << 1) |
             (1 << 0); // P0.21, P0.19, P0.17, P0.16
  int COLS = (1 << 24) | (1 << 22) | (1 << 20) |
             (1 << 18); // P0.22, P0.20, P0.18, P0.15

  int rows[4] = {3, 2, 1, 0};
  int cols[4] = {24, 22, 20, 18};
  // Sprawdzanie przerwań dla wierszy (przy zboczu opadającym lub narastającym)
  for (int row = 0; row < 4; row++) {
    int rowPin =
        rows[row]; // Obliczanie pinu wiersza: P0.21, P0.19, P0.17, P0.16
	
    if (LPC_GPIOINT->IO0IntStatF &
        (1 << rowPin)) { // Zbocze opadające (naciśnięcie klawisza)
      // Obsługa naciśnięcia klawisza
			if(row == 0){UART_SEND("row: 0 ", 8);}
			if(row == 1){UART_SEND("row: 1 ", 8);}
			if(row == 2){UART_SEND("row: 2 ", 8);}
			if(row == 3){UART_SEND("row: 3 ", 8);}

		
	
      for (int col = 0; col < 4; col++) {
				UART_SEND("no col", 6);
				if(col == 0){UART_SEND("col: 0\n", 8);}
				if(col == 1){UART_SEND("col: 1\n", 8);}
				if(col == 2){UART_SEND("col: 2\n", 8);}
				if(col == 3){UART_SEND("col: 3\n", 8);}
        int colPin =
            cols[col]; // Obliczanie pinu kolumny: P0.22, P0.20, P0.18, P0.15

        if (!(LPC_GPIO0->FIOPIN & (1 << colPin))) { // Jeśli stan kolumny niski
          keyState[col][row] = true;                // Klawisz wciśnięty
		
					UART_SEND(&keypadChars[col][row], 1);
        }
      }
    }
		
    if (LPC_GPIOINT->IO0IntStatR &
        (1 << rowPin)) { // Zbocze narastające (puszczenie klawisza)
      // Obsługa puszczenia klawisza
      for (int col = 0; col < 4; col++) {
 
				int colPin = cols[col];
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
