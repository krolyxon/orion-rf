#pragma once
#include <Arduino.h>

void badUSBMenu(int index);
void showRunningScreen(String taskName, uint8_t duration);
void runCommand(const char *command);
