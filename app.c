#include "app.h"
int x_wsk;
int y_wsk;
bool keyState[4][4] = {{false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false},
                       {false, false, false, false}};
volatile uint32_t msTicks = 0;
volatile uint32_t lastScreenDebounce = 0;
const uint32_t debounceDelay = 50;
const char keypadChars[4][4] = {{'1', '2', '3', 'A'},
                                {'4', '5', '6', 'B'},
                                {'7', '8', '9', 'C'},
                                {'*', '0', '#', 'D'}};
