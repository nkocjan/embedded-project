
#ifndef INTERFACE_H
#define INTERFACE_H

#include "app.h"
#include "lcd.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define MAX_CODE_LENGTH 4
#define MAX_DATE_LENGTH 12

int codeWriten = 0;
int actualInterface = 0;
int newDateTab[MAX_DATE_LENGTH] = {0};
int newDateTabId = 0;

void processInput(char keys[16]) {
  int count = 0;
  for (int i = 0; i < 16; i++) {
    if (keys[i] != ' ') {
      count++;
    }
  }

  switch (count) {
  case 0:
    return;
  case 1:
    // processOneKey();
    break;
  case 2:
    // processTwoKeys();
    break;
  default:
    return;
  }
}

void drawLastOpenDates() {
  drawString(40, 170, "Ostatnie otwarcia:");
  for (int i = 0; i < 6; i++) {
    drawString(40, 200 + i * 15, "13:45 17.12.2024");
  }
}

void drawStar(int x, int y, int position) {
  x += position * 40;
  drawLine(x - 10, y - 10, x + 10, y + 10);
  drawLine(x + 10, y - 10, x - 10, y + 10);
  drawLine(x, y - 10, x, y + 10);
  drawLine(x - 10, y, x + 10, y);
}

void drawDateInTopRight() {
  const int x = 240 - 80 - 20;
  const int y = 20;
  char timeStr[30];
  snprintf(timeStr, sizeof(timeStr), "%02d:%02d", hours,
           minutes); // Use snprintf for safety
  drawString(x, y, timeStr);
}

void drawBasicInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  // Box for stars
  drawLine(40, 150, 200, 150);
  drawLine(40, 150, 40, 50);
  drawLine(200, 150, 200, 50);
  drawLine(40, 50, 200, 50);

  // Code input placeholders
  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    drawLine(50 + i * 40, 100, 70 + i * 40, 100);
  }

  drawDateInTopRight();
  drawLastOpenDates();
}

void setNewCodeInterface() {
  setAutoIncrementBackground();

  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 30, "Podaj nowy kod");

  for (int i = 0; i < MAX_CODE_LENGTH; i++) {
    drawLine(50 + i * 40, 100, 70 + i * 40, 100);
  }
}

void setNewCodeSuccessfulInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Poprawnie dodano kod");
}

void setNewDateInterface() {
  setAutoIncrementBackground();

  drawString(40, 20, "Podaj rok");
  drawLine(40, 50, 140, 50);

  drawString(40, 70, "Podaj miesiac");
  drawLine(40, 100, 80, 100);

  drawString(40, 120, "Podaj dzien");
  drawLine(40, 150, 80, 150);

  drawString(40, 170, "Podaj godzine");
  drawLine(40, 200, 80, 200);

  drawString(40, 220, "Podaj minute");
  drawLine(40, 250, 80, 250);
}

void setNewDateInterfaceSuccessful() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Poprawnie zapisano nową datę");
}

void codeFull() {
  if (actualInterface == 0) {
    drawSuccessfulCodeInterface();
    codeWriten = 0;
  } else if (actualInterface == 1) {
    setNewCodeSuccessfulInterface();
  } else if (actualInterface == 2 && newDateTabId == MAX_DATE_LENGTH) {
    setNewDateInterfaceSuccessful();
    memset(newDateTab, 0, sizeof(newDateTab)); // Reset newDateTab
    newDateTabId = 0;
  }
}

// Simplify functions func0-func9 by using a common helper
void handleKeyInput(int key) {
  if (actualInterface == 2 && newDateTabId < MAX_DATE_LENGTH) {
    int xPos = 40 + (newDateTabId % 4) * 30;
    int yPos = 48 + (newDateTabId / 4) * 50;
    char keyStr[2] = {key + '0', '\0'};
    drawString(xPos, yPos, keyStr);
    newDateTab[newDateTabId++] = key;
  } else if (actualInterface == 0 && codeWriten < MAX_CODE_LENGTH) {
    drawStar(60, 100, codeWriten++);
  }
}

void handleKeyPress(int key) {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
    // Draw stars for the code interface
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    // Handle new date input
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, (char[]){key + '0', '\0'});
    } else if (newDateTabId < 6) {
      drawString(40 + (newDateTabId - 4) * 30, 98, (char[]){key + '0', '\0'});
    } else if (newDateTabId < 8) {
      drawString(40 + (newDateTabId - 6) * 30, 148, (char[]){key + '0', '\0'});
    } else if (newDateTabId < 10) {
      drawString(40 + (newDateTabId - 8) * 30, 198, (char[]){key + '0', '\0'});
    } else if (newDateTabId < 12) {
      drawString(40 + (newDateTabId - 10) * 30, 248, (char[]){key + '0', '\0'});
    }

    if (newDateTabId < 12) {
      newDateTab[newDateTabId++] = key;
    }
  }
}

// Functions for key presses 0–9
void func0() { handleKeyPress(0); }
void func1() { handleKeyPress(1); }
void func2() { handleKeyPress(2); }
void func3() { handleKeyPress(3); }
void func4() { handleKeyPress(4); }
void func5() { handleKeyPress(5); }
void func6() { handleKeyPress(6); }
void func7() { handleKeyPress(7); }
void func8() { handleKeyPress(8); }
void func9() { handleKeyPress(9); }

// Other functions remain unchanged
void funcA() { codeFull(); }
void funcB() {
  actualInterface = 1;
  setNewCodeInterface();
}
void funcC() {
  actualInterface = 2;
  setNewDateInterface();
}
void funcD() {}
void funcE() {}
void funcF() {
  actualInterface = 0;
  codeWriten = 0;
  memset(newDateTab, 0, sizeof(newDateTab)); // Properly reset newDateTab
  newDateTabId = 0;
  drawBasicInterface();
}

#endif
