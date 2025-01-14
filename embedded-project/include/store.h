#ifndef RTC_STORAGE_H
#define RTC_STORAGE_H

#include "app.h"
#include "uart.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>

#define CODE_REGISTER LPC_RTC->GPREG0  // GPREG0: Unlock code (4 bytes)
#define DATE_REGISTER1 LPC_RTC->GPREG1 // GPREG1: First date (4 bytes)
#define DATE_REGISTER2 LPC_RTC->GPREG2 // GPREG2: Second date (4 bytes)
#define DATE_REGISTER3 LPC_RTC->GPREG3 // GPREG3: Third date (4 bytes)
#define DATE_REGISTER4 LPC_RTC->GPREG4 // GPREG4: Fourth date (4 bytes)

void setCode(int code[4]) {
  CODE_REGISTER = 0;
  for (int i = 0; i < 4; i++) {
    CODE_REGISTER |= ((code[i] & 0xF) << (i * 8));
  }
}

void getCode(int code[4]) {
  for (int i = 0; i < 4; i++) {
    code[i] = (CODE_REGISTER >> (i * 8)) & 0xF;
  }
}

void addNewDate(uint32_t newDate) {
  DATE_REGISTER4 = DATE_REGISTER3;
  DATE_REGISTER3 = DATE_REGISTER2;
  DATE_REGISTER2 = DATE_REGISTER1;
  DATE_REGISTER1 = newDate;
}

void getDates(uint32_t dates[4]) {
  dates[0] = DATE_REGISTER1;
  dates[1] = DATE_REGISTER2;
  dates[2] = DATE_REGISTER3;
  dates[3] = DATE_REGISTER4;
}

uint32_t encodeDateTime(int year, int month, int dayOfMonth, int hour,
                        int minute) {
  uint32_t encoded = 0;

  encoded |=
      (year - 2000) & 0x7F; // 7 bits for year (offset by 2000, range 0-127)
  encoded |= (month & 0xF) << 7;        // 4 bits for month (range 1-12)
  encoded |= (dayOfMonth & 0x1F) << 11; // 5 bits for day (range 1-31)
  encoded |= (hour & 0x1F) << 16;       // 5 bits for hour (range 0-23)
  encoded |= (minute & 0x3F) << 21;     // 6 bits for minute (range 0-59)

  return encoded;
}

// Function to decode a 4-byte integer into a formatted date string
void decodeDateTime(uint32_t encoded, char *dateBuffer) {
  int year = (encoded & 0x7F) + 2000;      // Extract year and add 2000 offset
  int month = (encoded >> 7) & 0xF;        // Extract month
  int dayOfMonth = (encoded >> 11) & 0x1F; // Extract day
  int hour = (encoded >> 16) & 0x1F;       // Extract hour
  int minute = (encoded >> 21) & 0x3F;     // Extract minute

  sprintf(dateBuffer, "%04d-%02d-%02d %02d:%02d", year, month, dayOfMonth, hour,
          minute);
}

#endif
