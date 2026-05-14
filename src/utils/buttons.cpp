#include "buttons.h"
#include "../config.h"
#include <Arduino.h>

void buttonsInit() {
    pinMode(BTN_UP, INPUT_PULLUP);
    pinMode(BTN_DOWN, INPUT_PULLUP);
    pinMode(BTN_SELECT, INPUT_PULLUP);
    pinMode(BTN_BACK, INPUT_PULLUP);
    pinMode(BTN_RIGHT, INPUT_PULLUP);
    pinMode(BTN_LEFT, INPUT_PULLUP);
}

bool btnUp() { return !digitalRead(BTN_UP); }
bool btnDown() { return !digitalRead(BTN_DOWN); }
bool btnSelect() { return !digitalRead(BTN_SELECT); }
bool btnBack() { return !digitalRead(BTN_BACK); }
bool btnRight() { return !digitalRead(BTN_RIGHT); }
bool btnLeft() { return !digitalRead(BTN_LEFT); }
