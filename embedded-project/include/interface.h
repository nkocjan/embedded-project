#ifndef INTERFACE_H
#define INTERFACE_H
#include <stdio.h>

#include <string.h>

#include <stdbool.h>

#include <math.h>

#include "lcd.h"
#include "app.h"


int codeWriten = 0;
int actualInterface = 0;
int newDateTab[12];
int newDateTabId = 0;



void drawLastOpenDates() {
  drawString(40, 170, "Ostatnie otwarcia:");
  for (int i = 0; i < 6; i++) {
    drawString(40, 200 + i * 15, "13:45 17.12.2024");
  }
}

void drawStar(int x, int y, int position) {
  x = x + position * 40;
  drawLine(x - 10, y - 10, x + 10, y + 10);
  drawLine(x + 10, y - 10, x - 10, y + 10);
  drawLine(x, y - 10, x, y + 10);
  drawLine(x - 10, y, x + 10, y);
}

void drawDateInTopRight() {
  const int x = 240 - 80 - 20;
  const int y = 20;
  char wsk[30];
  sprintf(wsk, "%d:%d", hours, minutes);
	UART_SEND(wsk, 20);
  drawString(x, y, wsk);
}

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

void setNewCodeInterface() {
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

  drawLine(50, 100, 70, 100);
  drawLine(90, 100, 110, 100);
  drawLine(130, 100, 150, 100);
  drawLine(170, 100, 190, 100);

  drawString(40, 30, "Podaj nowy kod");
}

void drawSuccessfulOpenInterface() {
  setAutoIncrementBackground();
  drawLine(10, 10, 230, 10);
  drawLine(10, 10, 10, 310);
  drawLine(230, 10, 230, 310);
  drawLine(10, 310, 230, 310);

  drawString(40, 40, "Otwarte");
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
    // Rysowanie otwarcia ekranu
    codeWriten = 0;
		drawSuccessfulOpenInterface();
  } else if (actualInterface == 1) {
    setNewCodeSuccessfulInterface();
  } else if (actualInterface == 2 && newDateTabId == 12) {
    setNewDateInterfaceSuccessful();
		int _year = newDateTab[0] * 1000 + newDateTab[1]*100 + newDateTab[2] * 10 + newDateTab[3] * 1;
		int _month = newDateTab[4] * 10 + newDateTab[5] * 1;
		int _day = newDateTab[6] * 10 + newDateTab[7] * 1;
		int godz = newDateTab[8] * 10 + newDateTab[9];
		int inuta = newDateTab[10] * 10 + newDateTab[11];
		set_hours(godz);
		set_minutes(inuta);

    for(int i = 0; i < 12; i++){
			newDateTab[i] = 0;
		};
    newDateTabId = 0;
  }
}

void func0() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "0");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "0");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "0");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "0");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "0");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 0;newDateTabId++;
    }
  }
}

void func1() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "1");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "1");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "1");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "1");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "1");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 1;
			newDateTabId++;
    }
  }
}
void func2() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "2");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "2");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "2");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "2");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "2");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 2;
			newDateTabId++;
    }
  }
}
void func3() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "3");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "3");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "3");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "3");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "3");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 3;
			newDateTabId++;
    }
  }
}
void func4() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "4");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "4");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "4");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "4");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "4");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 4;
			newDateTabId++;
    }
  }
}
void func5() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "5");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "5");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "5");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "5");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "5");
    }

    if (newDateTabId > 11) {
    } else {
     
      newDateTab[newDateTabId] = 5;
			newDateTabId++;
    }
  }
}
void func6() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "6");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "6");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "6");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "6");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "6");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 6;
			newDateTabId++;
    }
  }
}
void func7() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "7");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "7");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "7");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "7");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "7");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 7;
			newDateTabId++;
    }
  }
}
void func8() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "8");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "8");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "8");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "8");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "8");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 8;
			newDateTabId++;
    }
  }
}
void func9() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if ((codeWriten <= 3 && actualInterface == 0) || actualInterface == 1) {
    drawStar(60, 100, codeWriten);
    codeWriten++;
  } else if (actualInterface == 2) {
    if (newDateTabId < 4) {
      drawString(40 + newDateTabId * 30, 48, "9");
    } else if (newDateTabId == 4 || newDateTabId == 5) {
      drawString(40 + (newDateTabId - 4) * 30, 98, "9");
    } else if (newDateTabId == 6 || newDateTabId == 7) {
      drawString(40 + (newDateTabId - 6) * 30, 148, "9");
    } else if (newDateTabId == 8 || newDateTabId == 9) {
      drawString(40 + (newDateTabId - 8) * 30, 198, "9");
    } else if (newDateTabId == 10 || newDateTabId == 11) {
      drawString(40 + (newDateTabId - 10) * 30, 248, "9");
    }

    if (newDateTabId > 11) {
    } else {
      
      newDateTab[newDateTabId] = 9;
			newDateTabId++;
    }
  }
}
void funcA() { codeFull(); }
void funcB() {
	codeWriten = 0;
  actualInterface = 1;
  setNewCodeInterface();
}
void funcC() {
	newDateTabId = 0;
	  for(int i = 0; i < 12; i++){
			newDateTab[i] = -1;
		};
	codeWriten = 0;
  actualInterface = 2;
  setNewDateInterface();
}
void funcD() {}
void funcE() {}
void funcF() {
  actualInterface = 0;
  codeWriten = 0;
   for(int i = 0; i < 12; i++){
		newDateTab[i] = -1;
		};
  newDateTabId = 0;
  drawBasicInterface();
}

void processInput(char str) {
  switch (str) {
	case '0': {
		func0();
		break;
  }
  case '1': {
		func1();
		break;
  }
  case '2': {
		func2();
    // proccesOneKey();
    break;
  }
  case '3': {
		func3();
    // porcessTwoKeys()
    break;
  }
	case '4': {
		func4();
    // porcessTwoKeys()
    break;
  }
	case '5': {
		func5();
    // porcessTwoKeys()
    break;
  }
	case '6': {
		func6();
    // porcessTwoKeys()
    break;
  }
	case '7': {
		func7();
    // porcessTwoKeys()
    break;
  }
	case '8': {
		func8();
    // porcessTwoKeys()
    break;
  }
	case '9': {
		func9();
    // porcessTwoKeys()
    break;
  }
	case 'A': {
		funcA();
    // porcessTwoKeys()
    break;
  }
	case 'B': {
		funcB();
    // porcessTwoKeys()
    break;
  }
	case 'C': {
		funcC();
    // porcessTwoKeys()
    break;
  }
	case 'D': {
		funcD();
    // porcessTwoKeys()
    break;
  }
	case 'E': {
    funcE();
		// porcessTwoKeys()
    break;
  }
	case 'F': {
		funcF();
    // porcessTwoKeys()
    break;
  }
  default:
    return;
  }
}

#endif
