#include <Arduino.h>
#include <RF24.h>
#include "buttons.h"
#include "nrf24.h"
#include "display.h"
#define JAM_DURATION 500

extern SPIClass *RADIO_SPI;
extern RF24 radio1;
extern RF24 radio2;

const byte bleChannels[] = {2, 26, 80};

const byte bluetoothChannels[] = {
    32, 34, 46, 48, 50, 52,
    0, 1, 2, 4, 6, 8,
    22, 24, 26, 28, 30,
    74, 76, 78, 80
};

void initNRF(RF24 &radio)
{
    if (!radio.begin(RADIO_SPI)) {
        Serial.println("NRF not found");
        return;
    }

    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);
    radio.openWritingPipe(0xE7E7E7E7E7LL);
    Serial.println("NRF Initialized");

}



//void startBleJammer() {
//    initNRF(radio1);
//    initNRF(radio2);
//
//    Serial.println("NRF JAMMER STARTED");
//
//    const char payload[] = "xxxxxxxxxxxxxxxx";
//
//    u8g2.clearBuffer();
//    u8g2.drawStr(0, 10, "Jamming:");
//    u8g2.setCursor(60, 10);
//    u8g2.print("Bluetooth");
//    u8g2.sendBuffer();
//
//    while(true) {
//
//    // Channels (you can change this set)
//    const byte channels[] = {2, 26, 80};
//
//
//    for (int i = 0; i < sizeof(channels); i++) {
//        radio1.setChannel(channels[i]);
//        radio1.write(&payload, sizeof(payload));
//        //radio2.setChannel(channels[i]);
//        //radio2.write(&payload, sizeof(payload));
//    }
//
//     if (btnBack())
//    {
//        Serial.println("Jammer stopped");
//        return;
//    }
//    }
//
//}
//
//void startBluetoothJammer()
//{
//    initNRF(radio1);
//    initNRF(radio2);
//
//    Serial.println("NRF JAMMER STARTED");
//
//    const char payload[] = "xxxxxxxxxxxxxxxx";
//
//    u8g2.clearBuffer();
//    u8g2.drawStr(0, 10, "Jamming:");
//    u8g2.setCursor(60, 10);
//    u8g2.print("Bluetooth");
//    u8g2.sendBuffer();
//
//    while(true) {
//
//    // Channels (you can change this set)
//    const byte channels[] = {32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80};
//
//
//    for (int i = 0; i < sizeof(channels); i++) {
//        radio1.setChannel(channels[i]);
//        radio1.write(&payload, sizeof(payload));
//        //radio2.setChannel(channels[i]);
//        //radio2.write(&payload, sizeof(payload));
//    }
//
//     if (btnBack())
//    {
//        Serial.println("Jammer stopped");
//        return;
//    }
//    }
//}

void startJammer(const char* name, const byte* channels, size_t channelCount)
{
    initNRF(radio1);
    initNRF(radio2);

    Serial.println("NRF JAMMER STARTED");

    const char payload[] = "xxxxxxxxxxxxxxxx";

    u8g2.clearBuffer();
    u8g2.drawStr(0, 10, "Jamming:");
    u8g2.setCursor(60, 10);
    u8g2.print(name);
    u8g2.sendBuffer();

    while (true)
    {
        for (size_t i = 0; i < channelCount; i++)
        {
            radio1.setChannel(channels[i]);
            radio1.write(&payload, sizeof(payload));

            // Optional second NRF
            // radio2.setChannel(channels[i]);
            // radio2.write(&payload, sizeof(payload));
        }

        if (btnBack())
        {
            Serial.println("Jammer stopped");
            return;
        }
    }
}

void NRFToolsMenu(int index) {
    switch (index) {
        case 0:
            // startBleJammer();
            // BLE
            startJammer(
                "BLE",
                bleChannels,
                sizeof(bleChannels)
            );

            break;
        case 1:
            // startBluetoothJammer();
            // Bluetooth
            startJammer(
                "Bluetooth",
                bluetoothChannels,
                sizeof(bluetoothChannels)
            );
            break;
    }
}
