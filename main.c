#include "Open1768_LCD.h"
#include "LCD_ILI9325.h"
#include "LPC17xx.h"
#include "Driver_USART.h"
#include <Board_LED.h>
#include <Board_Buttons.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <math.h>
#include "asciiLib.h"
#include "TP_Open1768.h"

#define LCD_ILI9325 1

struct Point{
    int x;
    int y;
} typedef Point;

int x_wsk;
int y_wsk;

volatile uint32_t msTicks = 0;

volatile uint32_t lastScreenDebounce =0;

extern ARM_DRIVER_USART Driver_USART1;

static ARM_DRIVER_USART *USARTdrv = &Driver_USART1;

const uint32_t debounceDelay = 50;

void SysTick_Handler(void) {
    msTicks++;
}

void UART_SEND(const char * wsk, int len){
    USARTdrv->Send(wsk, len);
}

void TIMER0_IRQHandler(){
    UART_SEND("PING\n", 5);
    LPC_TIM0->IR = 1;
}

int debounceButton(int currentState, int *lastState, uint32_t *lastDebounceTime) {
    if (currentState != *lastState) {
        *lastDebounceTime = msTicks;
    }
    if ((msTicks - *lastDebounceTime) > debounceDelay) {
        if (currentState != *lastState) {
            *lastState = currentState;
            return currentState;
        }
    }
    return -1;
}

void myUART_Thread(const void *args) {
    char cmd;
    USARTdrv->Send("\nPress Enter to receive a message", 34);
    while (1) {
        while (USARTdrv->GetStatus().tx_busy) {}
        USARTdrv->Receive(&cmd, 1);
        while (USARTdrv->GetStatus().rx_busy) {}

        if (cmd == 13) {
            USARTdrv->Send("\nHello World!", 12);
        }
    }
}

void initalizeDISPLAY(){
    LPC_GPIOINT->IO0IntEnF = (1 << 19);
    NVIC_EnableIRQ(EINT3_IRQn);
}


// Inicjacja konfiguracji LCD
// wysylanie danych na UART
void initLcdConfiguration(){
    lcdConfiguration();
    int lc = lcdReadReg(0);

    if(lc == 0x8989){
        UART_SEND("SDD1289",7);
    }
    else if(lc == 0x9328){
        UART_SEND("ILI9325",7);
    }
    init_ILI9325();
}

// INICJALIZACJA
void initialize() {
    SysTick_Config(SystemCoreClock / 1000);
    initalizeDISPLAY();
    initLcdConfiguration();
    touchpanelInit();
    USARTdrv->Initialize(NULL);
    USARTdrv->PowerControl(ARM_POWER_FULL);
    USARTdrv->Control(ARM_USART_MODE_ASYNCHRONOUS |
                      ARM_USART_DATA_BITS_8 |
                      ARM_USART_PARITY_NONE |
                      ARM_USART_STOP_BITS_1 |
                      ARM_USART_FLOW_CONTROL_NONE, 4800);
    USARTdrv->Control(ARM_USART_CONTROL_TX, 1);
    USARTdrv->Control(ARM_USART_CONTROL_RX, 1);
}

// Inicjalizuj RTC
void initializeRTC(){
    LPC_SC->PCONP |= (1 << 9);
    LPC_RTC->CCR = 1;
    LPC_RTC->ILR = 0;
    LPC_RTC->CIIR = 1;

    NVIC_EnableIRQ(RTC_IRQn);
}


// Handler do RTC
void RTC_IRQHandler(){
    static int tikTak = 1;

    if(tikTak){
        UART_SEND("TIK ",4);
        tikTak = 0;
    }
    else{
        UART_SEND("TAK ",4);
        tikTak = 1;
    }

    LPC_RTC->ILR = 1;
}

void EINT0_IRQHandler(){
    static int led = 1;
    if(LPC_SC->EXTINT & 1 ){
        led = (led == 1 ?  0 : 1);
        UART_SEND("Click\n", 6);
    }
    LPC_SC->EXTINT = 1;
}

void firePixel(int x, int y, int color){
    lcdWriteReg(ADRX_RAM, x);
    lcdWriteReg(ADRY_RAM, y);
    lcdWriteReg(DATA_RAM, color);
}





// Szybkie kolorowanie tla
void setAutoIncrementBackground(){
    lcdWriteReg(HADRPOS_RAM_START, 0);
    lcdWriteReg(HADRPOS_RAM_END, LCD_MAX_X);
    lcdWriteReg(VADRPOS_RAM_START, 0);
    lcdWriteReg(VADRPOS_RAM_END, LCD_MAX_Y);
    lcdWriteIndex(DATA_RAM);

    for(int i = 0; i < LCD_MAX_X; i++){
        for(int j = 0; j < LCD_MAX_Y; j++){
            lcdWriteData(LCDGinger);
        }
    }
}






void drawLetter(Point p1, const char letter) {
    unsigned char buff[16];  // Tablica do przechowywania danych litery (16x16)
    GetASCIICode(1, buff, letter);  // Zakladam, ze ta funkcja laduje kod ASCII dla litery

    for (int y = 0; y < 16; y++) {  // Iteracja po wierszach (wysokosc)
        for (int x = 0; x < 16; x++) {  // Iteracja po kolumnach (szerokosc)
            if (buff[y] & (0x80 >> x)) {  // Sprawdzamy, czy dany bit jest ustawiony na 1
                firePixel(p1.x + x, p1.y + y, LCDBlue);  // Rysowanie pikseli
            }
        }
    }
}


//nie
void drawString(Point p1, const char* wsk){
    int iter = 0;
    while (*wsk) {
        Point p2;
        p2.x = p1.x + iter * 8;
        p2.y = p1.y;
        drawLetter(p2, *wsk);
        wsk++;
        iter++;
    }
}

Point calibrate(Point _get){
    int tp_xMax = 4095;
    int tp_yMax = 4095;

    Point tmp;

    // to do
    // 90 stopni
        // temp_x = y_touch;
        // temp_y = x_touch;
    // 180 stopni
        // temp_x = 4000 - x_touch;
        // temp_y = 4000 - y_touch;
    // 270 stopni
        // temp_x = 4000 - y_touch;
        // temp_y = x_touch;

    tmp.x = (_get.x * LCD_MAX_X) / tp_xMax;
    tmp.y = (_get.y * LCD_MAX_Y) / tp_yMax;

    return tmp;
}


void drawLine(const Point p1, const Point p2)
 {
     int d, dx, dy, ai, bi, xi, yi;
     int x = p1.x, y = p1.y;
     if (p1.x < p2.x){
         xi = 1;
         dx = p2.x - p1.x;
    }
     else{
         xi = -1;
         dx = p1.x - p2.x;
     }
     if (p1.y < p2.y){
         yi = 1;
         dy = p2.y - p1.y;
     }
     else{
         yi = -1;
         dy = p1.y - p2.y;
     }
     firePixel(x,y, LCDBlue);
     if (dx > dy){
         ai = (dy - dx) * 2;
         bi = dy * 2;
         d = bi - dx;
         while (x != p2.x){
             if (d >= 0){
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else
             {
                 d += bi;
                 x += xi;
             }
             firePixel(x,y, LCDBlue);
         }
     }
     else
     {
         ai = ( dx - dy ) * 2;
         bi = dx * 2;
         d = bi - dy;
         while (y != p2.y)
         {
             if (d >= 0){
                 x += xi;
                 y += yi;
                 d += ai;
             }
             else{
                 d += bi;
                 y += yi;
             }
             firePixel(x,y, LCDBlue);
         }
     }
 }

void draw_keyboard(){
    int x = LCD_MAX_X;
    int y = LCD_MAX_Y / 2;

    Point p1;p1.x = 0;p1.y=y;
    Point p2;p2.x = x;p2.y=y;

    drawLine(p1, p2);
}


int main(void){
    initialize();
		setAutoIncrementBackground();
    draw_keyboard();

    while(1){}
}
