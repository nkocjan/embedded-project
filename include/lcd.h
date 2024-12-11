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

#endif
