#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "lcd.h"
#include "app.h"

void showLastOpenDate(int position){
  drawString(40, 210 + position * 20, "13:45 17.12.2024");
}

void drawStar(int x, int y){
  drawLine(x - 10, y - 10, x + 10, y + 10);
  drawLine(x + 10, y - 10, x - 10, y + 10);
  drawLine(x, y - 10, x, y + 10);
}

void showActualDate(){
  const int x = 240 - 80 - 20;
  const int y = 20;
	char wsk[30];
	sprintf(wsk, "%d:%d", hours,minutes);
  drawString(x,y,wsk);
}

void showInterface(){
  setAutoIncrementBackground();
	
	//Rysowanie box na interfejs
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  //Rysowanie boxa na gwiazdki
  drawLine(40, 150, 200, 150);
  drawLine(40, 150, 40, 50);
  drawLine(200, 150, 200, 50);
  drawLine(40, 50, 200, 50);

  //Podkladka pod kod
  drawLine(50, 100, 70, 100);
  drawLine(90, 100, 110, 100);
  drawLine(130, 100, 150, 100);
  drawLine(170, 100, 190, 100);
	
	drawString(40, 170, "Last opened:");
}

void drawCodeEntry(int success){
  if(success){
    setAutoIncrementBackground();
    drawString(90, 100, "Opened");
  }else {
    showInterface();
  }
}

void drawSettingCodeInterface(){
	 setAutoIncrementBackground();
	
	//Rysowanie box na interfejs
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);
	
  //Rysowanie boxa na gwiazdki
  drawLine(40, 150, 200, 150);
  drawLine(40, 150, 40, 50);
  drawLine(200, 150, 200, 50);
  drawLine(40, 50, 200, 50);

  //Podkladka pod kod
  drawLine(50, 100, 70, 100);
  drawLine(90, 100, 110, 100);
  drawLine(130, 100, 150, 100);
  drawLine(170, 100, 190, 100);
	
	drawString(40, 30, "Set new code:");
}





#endif