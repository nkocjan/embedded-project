#ifndef INTERFACE_H
#define INTERFACE_H

#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "lcd.h"
#include "rtc.h"
#include "app.h"
#include "handlers.h"
#include "store.h"
#include "engine.h"

int actualInterface = 0;

int codeTab[4];
int codeTabId = 0;

int newDateTab[12];
int newDateTabId = 0;

// Temporary items
int aktualnyKod[4] = {0,0,0,0};

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
	
  getCode(aktualnyKod);
	
	char tmp[10];
	sprintf(tmp,"code: %d%d%d%d",aktualnyKod[0],aktualnyKod[1],aktualnyKod[2],aktualnyKod[3]);
	UART_SEND(tmp,11);
  for(int i = 0; i < 4; i++){
    if(codeTab[i] != aktualnyKod[i]){
      return 0;
    }
  }
  return 1;
}

// Ostatnie otwarcia
void drawLastOpenDates() {
  drawString(40, 170, "Ostatnie otwarcia:");

  uint32_t lastOpenDates[4];
  getDates(lastOpenDates);
  char dateBuf[30];

  decodeDateTime(lastOpenDates[0], dateBuf);
  drawString(40, 200, dateBuf);
  decodeDateTime(lastOpenDates[1], dateBuf);
  drawString(40, 215, dateBuf);
  decodeDateTime(lastOpenDates[2], dateBuf);
  drawString(40, 230, dateBuf);
  decodeDateTime(lastOpenDates[3], dateBuf);
  drawString(40, 245, dateBuf);
}
// Rysowanie gwiazdki do kodu
void drawStar(int x, int y, int position){
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

// Interfejs błedna data
void drawWrongDateInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Bledny format daty");
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

void funcF() {
  resetCodeTab();
  resetDateTab();
  actualInterface = 0;
  drawBasicInterface();
}

// Wywołanie klawisza A
void approve() {
  if (actualInterface == 0 && codeTabId == 4) {
    if(validateCode()){

      int _year = get_year();
      int _month = get_month();
      int _day = get_day();
      int _hour = get_hours();
      int _min = get_minutes();
      
      uint32_t tmpTime = encodeDateTime(_year, _month, _day, _hour, _min);
      addNewDate(tmpTime);

      drawSuccessfulOpenInterface();
			startEngine();
			wait(5000);   
			stopEngine();
      funcF();
    }else {
      drawWrongCodeOpenInterface();
    }
		
  } else if (actualInterface == 1 && codeTabId == 4) {
		char tmp[10];
		sprintf(tmp,"code: %d%d%d%d",codeTab[0],codeTab[1],codeTab[2],codeTab[3]);
		UART_SEND(tmp,11);
    setCode(codeTab);
    getCode(aktualnyKod);
    setNewCodeSuccessfulInterface();
    resetCodeTab();
    resetDateTab();
		wait(5000);
		funcF();
  } else if (actualInterface == 2 && newDateTabId == 12) {
		int _year = newDateTab[0] * 1000 + newDateTab[1]*100 + newDateTab[2] * 10 + newDateTab[3] * 1;
		int _month = newDateTab[4] * 10 + newDateTab[5] * 1;
		int _day = newDateTab[6] * 10 + newDateTab[7] * 1;
		int _hour = newDateTab[8] * 10 + newDateTab[9];
		int _min = newDateTab[10] * 10 + newDateTab[11];

    if(_min < 0 || _min > 59 || _hour < 0 || _hour > 59 || _month < 0 || _month > 12 ){
      drawWrongDateInterface();
      return;
    }
		
    set_years(_year);
    set_month(_month);
    set_day(_day);
    set_hours(_hour);
		set_minutes(_min);

    setNewDateInterfaceSuccessful();
    
    resetCodeTab();
    resetDateTab();
  }
}

void func(int value) {
	
	
  if (codeTabId > 3) {
    resetCodeTab();
		funcF();
  } else if (actualInterface == 0 || actualInterface == 1) {
    drawStar(60, 100, codeTabId);
    codeTab[codeTabId] = value;
    codeTabId++;

  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, (char[2]){value + '0', '\0'});
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, (char[2]){value + '0', '\0'});
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, (char[2]){value + '0', '\0'});
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, (char[2]){value + '0', '\0'});
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, (char[2]){value + '0', '\0'});
    }

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





#endif
