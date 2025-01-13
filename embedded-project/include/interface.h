#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "lcd.h"
#include "app.h"

int actualInterface = 0;

int codeTab[4];
int codeTabId = 0;

int newDateTab[12];
int newDateTabId = 0;

// Temporary items
int aktualnyKod[4] = {1,2,3,4};
int ostatnieOtwarcia[16] = {9,8,13,44,10,6,16,22};

// Ostatnie otwarcia
void drawLastOpenDates() {
  drawString(40, 170, "Ostatnie otwarcia:");
  drawString(40, 200,"17.12 13:45");
  drawString(40, 215,"17.12 17:66");
}

// Rysowanie gwiazdki do kodu
void drawStar(int x, int y, int position) {
  x = x + position * 40;
  drawLine(x - 10, y - 10, x + 10, y + 10);
  drawLine(x + 10, y - 10, x - 10, y + 10);
  drawLine(x, y - 10, x, y + 10);
  drawLine(x - 10, y, x + 10, y);
}

// Rysowanie aktualnej daty w prawym górnym rogu
void drawDateInTopRight() {
  const int x = 140;
  const int y = 20;
  char wsk[30];
  sprintf(wsk, "%d:%d", hours, minutes);
	UART_SEND(wsk, 6);
  drawString(x, y, wsk);
}

// Rysowanie podstawowego interfaceu
void drawBasicInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  // Rysowanie boxa na gwiazdki
  drawLine(40, 150, 200, 150);
  drawLine(40, 150, 40, 50);
  drawLine(200, 150, 200, 50);
  drawLine(40, 50, 200, 50);

  // Podkladka pod kod
  drawLine(50, 100, 70, 100);
  drawLine(90, 100, 110, 100);
  drawLine(130, 100, 150, 100);
  drawLine(170, 100, 190, 100);
  drawDateInTopRight();
  drawLastOpenDates();
}

// Interfejs pod nowy kod
void setNewCodeInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);
  drawLine(40, 150, 200, 150);
  drawLine(40, 150, 40, 50);
  drawLine(200, 150, 200, 50);
  drawLine(40, 50, 200, 50);
  drawLine(50, 100, 70, 100);
  drawLine(90, 100, 110, 100);
  drawLine(130, 100, 150, 100);
  drawLine(170, 100, 190, 100);
  drawString(40, 30, "Podaj nowy kod");
}

// Interfejs oznaczający że jest otwarte
void drawSuccessfulOpenInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Otwarte");
}

// Interfejs informujący o błędnym kodzie
void drawWrongCodeOpenInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Bledny kod.");
  drawString(40, 80, "Wcisnij F aby");
  drawString(40, 100, "powrocic na start");
}

// Interfejs oznaczający poprawne ustawienie nowego kodu
void setNewCodeSuccessfulInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Poprawnie dodano kod");
}

// Interfejs ustawiający nową datę
void setNewDateInterface() {
  setAutoIncrementBackground();

  drawString(40, 20, "Podaj rok");

  drawLine(40, 50, 50, 50);
  drawLine(70, 50, 80, 50);
  drawLine(100, 50, 110, 50);
  drawLine(130, 50, 140, 50);

  drawString(40, 70, "Podaj miesiac");

  drawLine(40, 100, 50, 100);
  drawLine(70, 100, 80, 100);

  drawString(40, 120, "Podaj dzien");

  drawLine(40, 150, 50, 150);
  drawLine(70, 150, 80, 150);

  drawString(40, 170, "Podaj godzine");

  drawLine(40, 200, 50, 200);
  drawLine(70, 200, 80, 200);

  drawString(40, 220, "Podaj minute");

  drawLine(40, 250, 50, 250);
  drawLine(70, 250, 80, 250);
}

// Interfejs oznaczający poprawne dodanie nowej daty
void setNewDateInterfaceSuccessful() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Poprawnie zapisano nową datę");
}

// Wywołanie klawisza A
void approve() {
  if (actualInterface == 0 && codeTabId == 3) {
    if(validateCode()){
      drawSuccessfulOpenInterface();
      // TODO DOODAĆ PRZERWĘ 5 SEC
      funcF();
    }else {
      drawWrongCodeOpenInterface();
    }

		
  } else if (actualInterface == 1 && codeTabId == 3) {
    // TODO DODAC SET NEW CODE
    // Zapis do zmiennej aktualnyKod, jak i do pamięci
    // Na podstawie daty z RTC dodać zapis ostatniego wejścia
    setNewCodeSuccessfulInterface();
    resetCodeTab();
    resetDateTab();
  } else if (actualInterface == 2 && newDateTabId == 12) {
		int _year = newDateTab[0] * 1000 + newDateTab[1]*100 + newDateTab[2] * 10 + newDateTab[3] * 1;
		int _month = newDateTab[4] * 10 + newDateTab[5] * 1;
		int _day = newDateTab[6] * 10 + newDateTab[7] * 1;
		int godz = newDateTab[8] * 10 + newDateTab[9];
		int inuta = newDateTab[10] * 10 + newDateTab[11];
		set_hours(godz);
		set_minutes(inuta);
    setNewDateInterfaceSuccessful();
    
    resetCodeTab();
    resetDateTab();
  }
}

void func(int value) {
  if (codeTabId > 3) {
    resetCodeTab();

  } else if (codeTabId == 3 && (actualInterface == 0 || actualInterface == 1)) {
    drawStar(60, 100, codeTabId);
    codeTab[codeTabId] = value;
    codeTabId++;

  } else if (actualInterface == 2) {
    int baseX = 40 + (newDateTabId % 2) * 30;
    int baseY = 48 + ((newDateTabId / 2) % 5) * 50;

    drawString(baseX, baseY, (char[2]){value + '0', '\0'});

    if (newDateTabId <= 11) {
      newDateTab[newDateTabId] = value;
      newDateTabId++;
    }
  }
}

void funcA() { approve(); }
void funcB() {
  resetCodeTab();
	resetDateTab();
  actualInterface = 1;
  setNewCodeInterface();
}
void funcC() {
	resetCodeTab();
  resetDateTab();
  actualInterface = 2;
  setNewDateInterface();
}
void funcD() {}
void funcE() {}
void funcF() {
  resetCodeTab();
  resetDateTab();
  actualInterface = 0;
  drawBasicInterface();
}

void processInput(char str) {
  switch (str) {
	case '0': {
		func(0);
		break;
  }
  case '1': {
		func(1);
		break;
  }
  case '2': {
		func(2);
    break;
  }
  case '3': {
		func(3);
    break;
  }
	case '4': {
		func(4);
    break;
  }
	case '5': {
		func(5);
    break;
  }
	case '6': {
		func(6);
    break;
  }
	case '7': {
		func(7);
    break;
  }
	case '8': {
		func(8);
    break;
  }
	case '9': {
		func(9);
    break;
  }
	case 'A': {
		funcA();
    break;
  }
	case 'B': {
		funcB();
    break;
  }
	case 'C': {
		funcC();
    break;
  }
	case 'D': {
		funcD();
    break;
  }
	case 'E': {
    funcE();
    break;
  }
	case 'F': {
		funcF();
    break;
  }
  default:
    return;
  }
}

void resetCodeTab(){
  for(int i = 0; i < 4; i++){
    codeTab[i]=-1;
  }
  codeTabId = 0;
}

void resetDateTab(){
  for(int i = 0; i < 12; i++){
		newDateTab[i] = -1;
	};
  newDateTabId = 0;
}

int validateCode(){
  // TODO get kod z pamięci
  for(int i = 0; i < 4; i++){
    if(codeTab[i] != aktualnyKod[i]){
      return 0;
    }
  }
  return 1;
}

#endif
