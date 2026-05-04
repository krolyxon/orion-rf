#pragma once

#include <RF24.h>

// Initialization
void initNRF(RF24 &radio);

// Jammer modes
void startNRFJammer();        // dual-radio sweep jammer
void stopNRFJammer();         // (optional, for future)

// Advanced (later)
void nrfJammerSweep();        // internal, but can expose if needed
void nrfSetChannel(uint8_t ch);
