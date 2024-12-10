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
#include <string.h>

#define LCD_ILI9325 1

// ========================================
// ----- PINY -----
// ========================================
/*
kolumny -> pin P2_8 P2_6 P2_4 P2_2
                                         col 1		 2		 3   4
wiersze -> pin  P2_7 P2_5 P2_3  P2_1   row 1     2     3     4


OS X krotsza

*/

// ========================================
//===============ZMIENNE GLOBALNE====================
// ========================================
int x_wsk;
int y_wsk;

bool keyState[4][4];

volatile uint32_t msTicks = 0;

volatile uint32_t lastScreenDebounce = 0;

extern ARM_DRIVER_USART Driver_USART1;

static ARM_DRIVER_USART *USARTdrv = &Driver_USART1;

const uint32_t debounceDelay = 50;

// ========================================
//================STRUCTY =================
// ========================================

struct Point {
  int x;
  int y;
} typedef Point;

// ========================================
// ========== Handlery ====================
// ========================================

void SysTick_Handler(void) { msTicks++; }

void TIMER0_IRQHandler() {
  UART_SEND("PING\n", 5);
  LPC_TIM0->IR = 1;
}

// Handler do RTC
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

// ========================================
// ============= INICJALIZACJE ================
// ========================================

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
// Inicjacja konfiguracji LCD
// wysylanie danych na UART
void initLcdConfiguration() {
  lcdConfiguration();
  int lc = lcdReadReg(0);

  if (lc == 0x8989) {
    UART_SEND("SDD1289", 7);
  } else if (lc == 0x9328) {
    UART_SEND("ILI9325", 7);
  }
  init_ILI9325();
}

// Inicjalizuj RTC
void initializeRTC() {
  LPC_SC->PCONP |= (1 << 9);
  LPC_RTC->CCR = 1;
  LPC_RTC->ILR = 0;
  LPC_RTC->CIIR = 1;

  NVIC_EnableIRQ(RTC_IRQn);
}

// INICJALIZACJA
void initialize() {
  SysTick_Config(SystemCoreClock / 1000);
  initLcdConfiguration();
  touchpanelInit();

  // init GPIO
  initializeKeyboard();

  USARTdrv->Initialize(NULL);
  USARTdrv->PowerControl(ARM_POWER_FULL);
  USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS | ARM_USART_DATA_BITS_8 |
                        ARM_USART_PARITY_NONE | ARM_USART_STOP_BITS_1 |
                        ARM_USART_FLOW_CONTROL_NONE,
                    4800);
  USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
  USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
}

//=========================================
//================Funkcje =================
//=========================================

void UART_SEND(const char *wsk, int len) { USARTdrv->Send(wsk, len); }

int debounceButton(int currentState, int *lastState,
                   uint32_t *lastDebounceTime) {
  if (currentState != *lastState) {
    *lastDebounceTime = msTicks;
  }
  if ((msTicks - *lastDebounceTime) > debounceDelay) {
    if (currentState != *lastState) {
      *lastState = currentState;
      return currentState;
    }
  }
  return -1;
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

void firePixel(int x, int y, int color) {
  lcdWriteReg(ADRX_RAM, x);
  lcdWriteReg(ADRY_RAM, y);
  lcdWriteReg(DATA_RAM, color);
}

// Szybkie kolorowanie tla
void setAutoIncrementBackground() {
  lcdWriteReg(HADRPOS_RAM_START, 0);
  lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
  lcdWriteReg(VADRPOS_RAM_START, 0);
  lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
  lcdWriteIndex(DATA_RAM);

  for (int i = 0; i < LCD_MAX_X; i++) {
    for (int j = 0; j < LCD_MAX_Y; j++) {
      lcdWriteData(LCDGinger);
    }
  }
}

void drawLetter(Point p1, const char letter) {
  unsigned char buff[16]; // Tablica do przechowywania danych litery (16x16)
  GetASCIICode(1, buff,
               letter); // Zakladam, ze ta funkcja laduje kod ASCII dla litery

  for (int y = 0; y < 16; y++) {   // Iteracja po wierszach (wysokosc)
    for (int x = 0; x < 16; x++) { // Iteracja po kolumnach (szerokosc)
      if (buff[y] &
          (0x80 >> x)) { // Sprawdzamy, czy dany bit jest ustawiony na 1
        firePixel(p1.x + x, p1.y + y, LCDBlue); // Rysowanie pikseli
      }
    }
  }
}

// nie
void drawString(Point p1, const char *wsk) {
  int iter = 0;
  while (*wsk) {
    Point p2;
    p2.x = p1.x + iter * 8;
    p2.y = p1.y;
    drawLetter(p2, *wsk);
    wsk++;
    iter++;
  }
}

Point calibrate(Point _get) {
  int tp_xMax = 4095;
  int tp_yMax = 4095;

  Point tmp;

  // to do
  // 90 stopni
  // temp_x = y_touch;
  // temp_y = x_touch;
  // 180 stopni
  // temp_x = 4000 - x_touch;
  // temp_y = 4000 - y_touch;
  // 270 stopni
  // temp_x = 4000 - y_touch;
  // temp_y = x_touch;

  tmp.x = (_get.x * LCD_MAX_X) / tp_xMax;
  tmp.y = (_get.y * LCD_MAX_Y) / tp_yMax;

  return tmp;
}

void drawLine(const Point p1, const Point p2) {
  int d, dx, dy, ai, bi, xi, yi;
  int x = p1.x, y = p1.y;
  if (p1.x < p2.x) {
    xi = 1;
    dx = p2.x - p1.x;
  } else {
    xi = -1;
    dx = p1.x - p2.x;
  }
  if (p1.y < p2.y) {
    yi = 1;
    dy = p2.y - p1.y;
  } else {
    yi = -1;
    dy = p1.y - p2.y;
  }
  firePixel(x, y, LCDBlue);
  if (dx > dy) {
    ai = (dy - dx) * 2;
    bi = dy * 2;
    d = bi - dx;
    while (x != p2.x) {
      if (d >= 0) {
        x += xi;
        y += yi;
        d += ai;
      } else {
        d += bi;
        x += xi;
      }
      firePixel(x, y, LCDBlue);
    }
  } else {
    ai = (dx - dy) * 2;
    bi = dx * 2;
    d = bi - dy;
    while (y != p2.y) {
      if (d >= 0) {
        x += xi;
        y += yi;
        d += ai;
      } else {
        d += bi;
        y += yi;
      }
      firePixel(x, y, LCDBlue);
    }
  }
}

int main(void) {
  initialize();
  setAutoIncrementBackground();
  while (1) {
  }
}
