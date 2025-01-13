#ifndef HANDLERS_H
#define HANDLERS_H

#include "app.h"
#include "uart.h"
#include <cstdint>


void SysTick_Handler(void) { msTicks++; }

void wait(uint32_t waitTimeMs) {
  msTicks = 0;
  while (msTicks < waitTimeMs) {
  }
}

#endif
