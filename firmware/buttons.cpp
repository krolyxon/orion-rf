#include "buttons.h"
#include <Arduino.h>

#define BTN_UP 4
#define BTN_DOWN 5
#define BTN_SELECT 6
#define BTN_BACK 7

void buttonsInit()
{
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);
}

bool btnUp() { return !digitalRead(BTN_UP); }
bool btnDown() { return !digitalRead(BTN_DOWN); }
bool btnSelect() { return !digitalRead(BTN_SELECT); }
bool btnBack() { return !digitalRead(BTN_BACK); }
