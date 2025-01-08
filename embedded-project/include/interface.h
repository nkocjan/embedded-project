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

void processInput(char keys[16]) {
  int count = 0;
  for (int i = 0; i < 16; i++) {
    if (keys[i] != ' ') {
      count++;
    }
  }

  switch (count) {
  case 0: {
    return;
  }
  case 1: {
    // proccesOneKey();
    break;
  }
  case 2: {
    // porcessTwoKeys()
    break;
  }
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
    drawSuccessfulCodeInterface();
    codeWriten = 0;
  } else if (actualInterface == 1) {
    setNewCodeSuccessfulInterface();
  } else if (actualInterface == 2 && newDateTabId == 12) {
    setNewDateInterfaceSuccessful();

    newDateTab = [];
    newDateTabId = 0;
  }
}

void func0() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 0;
    }
  }
}

void func1() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 0;
    }
  }
}
void func2() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 0;
    }
  }
}
void func3() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 3;
    }
  }
}
void func4() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 4;
    }
  }
}
void func5() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 5;
    }
  }
}
void func6() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 6;
    }
  }
}
void func7() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 7;
    }
  }
}
void func8() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 8;
    }
  }
}
void func9() {
  if (codeWriten > 3) {
    codeWriten = 0;
  } else if (codeWriten <= 3 && actualInterface == 0) {
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
      newDateTabId++;
      newDateTab[newDateTabId] = 9;
    }
  }
}
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
  newDateTab = [];
  newDateTabId = 0;
  drawBasicInterface();
}

#endif
