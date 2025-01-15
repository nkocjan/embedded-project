#ifndef RTC_H
#define RTC_H
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "app.h"
#include "uart.h"

volatile int minutes;
volatile int hours;
volatile int months;
volatile int years;
volatile int days;
volatile char date[30];

int get_year(){
	UART_SEND("log: get_year", 14);
	return LPC_RTC->YEAR;
}	

int get_month(){
	UART_SEND("log: get_month", 15);
	return LPC_RTC->MONTH;
}

int get_day(){
	UART_SEND("log: get_day", 13);
	return LPC_RTC->DOM;
}

int get_hours(){
	UART_SEND("log: get_hours", 15);
	return LPC_RTC->HOUR;
}

int get_minutes(){
	UART_SEND("log: get_min", 13);
	return LPC_RTC->MIN;
}

void set_years(int y){
	years = y;
	LPC_RTC->YEAR = y;
	UART_SEND("log: set_year", 14);
}

void set_month(int m){
	months = m;
	LPC_RTC->MONTH = m;
	UART_SEND("log: set_month", 15);
}

void set_day(int d){
	days = d;
	LPC_RTC->DOM=d;
	UART_SEND("log: set_day", 13);
}

void set_hours(int h){
	hours=h;
	LPC_RTC->HOUR=h;
	UART_SEND("log: set_hours", 15);
}

void set_minutes(int m){
	minutes=m;
	LPC_RTC->MIN=m;
	UART_SEND("log: set_min", 13);
}

void initTimer1(){
	LPC_TIM1->PR=0;
	LPC_TIM1->MR0=10*25000000-1; // Handler co 10 sec w celu testowania
	LPC_TIM1->MCR= 0b011;
	LPC_TIM1->TCR=1;
	NVIC_EnableIRQ(TIMER1_IRQn);
	UART_SEND("log: init timer", 16);
}

void initRTC(){
	initTimer1();
	LPC_SC->PCONP |= 1<<9;

	minutes=get_minutes();
	hours=get_hours();
	UART_SEND("log: init RTC", 14);
}

void TIMER1_IRQHandler(){ 
	LPC_TIM1->IR=1;
	hours=get_hours();
	minutes = get_minutes();
	months = get_month();
	years = get_year();

	sprintf(date, "log: data: %d:%d %d-%d", hours, minutes, months, years);
	UART_SEND(date, strlen(date));
}
#endif