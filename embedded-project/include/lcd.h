#ifndef LCD_H
#define LCD_H
#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "asciiLib.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdint.h>
#include "uart.h"
#include "app.h"

#define LCD_ILI9325 1

void initLcdConfiguration() {
  lcdConfiguration();
  int lc = lcdReadReg(0);

  if (lc == 0x8989) {
    UART_SEND("SDD1289", 7);
  } else  {
    UART_SEND("ILI9325", 7);
  }
  init_ILI9325();
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

void drawLetter(int _x, int _y, const char letter) {
  unsigned char buff[16]; // Tablica do przechowywania danych litery (16x16)
  GetASCIICode(1, buff,
               letter); // Zakladam, ze ta funkcja laduje kod ASCII dla litery

  for (int y = 0; y < 16; y++) {   // Iteracja po wierszach (wysokosc)
    for (int x = 0; x < 16; x++) { // Iteracja po kolumnach (szerokosc)
      if (buff[y] &
          (0x80 >> x)) { // Sprawdzamy, czy dany bit jest ustawiony na 1
        firePixel(_x + x, _y + y, LCDBlue); // Rysowanie pikseli
      }
    }
  }
}

void drawString(int x, int y, const char *wsk) {
  int iter = 0;
  while (*wsk) {
		int x2;
		int y2;
    x2 = x + iter * 8;
    y2 = y;
    drawLetter(x2, y2, *wsk);
    wsk++;
    iter++;
  }
}

void drawLine(int x1, int y1, int x2, int y2) {
  int d, dx, dy, ai, bi, xi, yi;
  int x = x1, y = y1;
  if (x1 < x2) {
    xi = 1;
    dx = x2 - x1;
  } else {
    xi = -1;
    dx = x1 - x2;
  }
  if (y1 < y2) {
    yi = 1;
    dy = y2 - y1;
  } else {
    yi = -1;
    dy = y1 - y2;
  }
  firePixel(x, y, LCDBlue);
  if (dx > dy) {
    ai = (dy - dx) * 2;
    bi = dy * 2;
    d = bi - dx;
    while (x != x2) {
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
    while (y != y2) {
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

#endif
