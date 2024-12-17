#include "Driver_USART.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Open1768_LCD.h"
#include "app.h"
#include "asciiLib.h"
#include "handlers.h"
#include "include/keyboard.h"
#include "keyboard.h"
#include "lcd.h"
#include "rtc.h"
#include "uart.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

void initialize() {
  SysTick_Config(SystemCoreClock / 1000);
  initializeUART2();
  initLcdConfiguration();
  touchpanelInit();
  initializeKeyboard();
}

int main(void) {
  initialize();
  setAutoIncrementBackground();

  while (1) {
    if (wasInterupted) {
      readKeyboard();
    }
  }
}
