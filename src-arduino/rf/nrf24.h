#pragma once

#include <RF24.h>

// Initialization
void initNRF(RF24 &radio);

void startBluetoothJammer();
void startBleJammer();

void startJammer(const char *name, const byte *channels, size_t channelCount);

void NRFToolsMenu(int index);
