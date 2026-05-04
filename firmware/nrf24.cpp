#include <Arduino.h>
#include <RF24.h>
#include "buttons.h"
#include "nrf24.h"
#include "display.h"
#define JAM_DURATION 500

extern SPIClass *RADIO_SPI;
extern RF24 radio1;
extern RF24 radio2;

void initNRF(RF24 &radio)
{
    radio.begin(RADIO_SPI);

    radio.setAutoAck(false);
    radio.stopListening();

    radio.setRetries(0, 0);
    radio.setDataRate(RF24_2MBPS);
radio.setPALevel(RF24_PA_MAX);

    radio.openWritingPipe(0xE7E7E7E7E7LL);
}


void jamChannels(const char* label, int startCh, int endCh) {
  byte data1[32], data2[32];
  for (int i = 0; i < 32; i++) {
    data1[i] = random(0, 256);
    data2[i] = random(0, 256);
  }

  for (int ch = startCh; ch <= endCh; ch++) {
    // Status screen
    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Jamming:");
    u8g2.setCursor(60, 10);
    u8g2.print(label);
    u8g2.setCursor(0, 30);
    u8g2.print("Channel: ");
    u8g2.print(ch);
    u8g2.sendBuffer();

    unsigned long startTime = millis();
    while (millis() - startTime < JAM_DURATION) {
      radio1.setChannel(ch);
      radio1.stopListening();
      radio1.write(data1, sizeof(data1));
      delayMicroseconds(100);

      radio2.setChannel(ch);
      radio2.stopListening();
      radio2.write(data2, sizeof(data2));
      delayMicroseconds(100);

  }
}
}


void nrfJammerSweep()
{
    static uint8_t ch1 = 0;
    static uint8_t ch2 = 124;

    uint8_t payload[32] = {0xFF};

    radio1.setChannel(ch1);
    radio1.writeFast(payload, sizeof(payload));

    radio2.setChannel(ch2);
    radio2.writeFast(payload, sizeof(payload));

    ch1++;
    ch2--;

    if (ch1 > 124) ch1 = 0;
    if (ch2 > 124) ch2 = 124;
}

void startNRFJammer()
{
    initNRF(radio1);
    initNRF(radio2);

    Serial.println("NRF JAMMER STARTED");
    // nrfJammerSweep();
      jamChannels("Bluetooth", 0, 78);


    if (btnBack())
    {
        Serial.println("Jammer stopped");
        return;
    }

    delayMicroseconds(200);
}
