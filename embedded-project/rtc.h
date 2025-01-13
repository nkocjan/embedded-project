#ifndef RTC_H
#define RTC_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "app.h"
#include "uart.h"

 int minutes;
 int hours;
volatile int months;
volatile int years;
volatile int days;
volatile char date[20];


int get_hours(){
	return LPC_RTC->HOUR;
}

int get_minutes(){
	return LPC_RTC->MIN;
}

int get_month(){
	return LPC_RTC->MONTH;
}

int get_year(){
	return LPC_RTC->YEAR;
}

void set_day(int d){
	days = d;
	LPC_RTC->DOM=d;
}

void set_hours(int h){
	hours=h;
	LPC_RTC->HOUR=h;
}

void set_minutes(int m){
	minutes=m;
	LPC_RTC->MIN=m;
}

void set_seconds(int s){
	LPC_RTC->SEC=s;
}

void set_month(int m){
	months = m;
	LPC_RTC->MONTH = m;
}

void set_years(int y){
	years = y;
	LPC_RTC->YEAR = y;
}

void initTimer1(){
	LPC_TIM1->PR=0; // without prescaler
	LPC_TIM1->MR0=10*25000000-1; // 1 MIN
	LPC_TIM1->MCR= 0b011; // with interrupt, reset timer on the match, don't stop timer at MR0
	LPC_TIM1->TCR=1; // enable timer
	NVIC_EnableIRQ(TIMER1_IRQn);
}

void initRTC(){
	initTimer1();
	LPC_SC->PCONP |= 1<<9; // PCRTC register - supply power to RTC (it's default on)
	minutes=get_minutes();
	hours=get_hours();
}


void TIMER1_IRQHandler(){ // update time
	LPC_TIM1->IR=1;
	hours=get_hours();
	minutes = get_minutes();
	months = get_month();
	years = get_year();

	sprintf(date, "%d:%d %d-%d", hours, minutes, months, years);
	UART_SEND(date, strlen(date));
}
#endif