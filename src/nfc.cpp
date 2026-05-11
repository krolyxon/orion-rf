#include "nfc.h"

#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_PN532.h>

#include "ui/display.h"
#include "buttons.h"
#include "config.h"

#define PN532_IRQ   -1
#define PN532_RESET -1

//Adafruit_PN532 nfc(Wire);
Adafruit_PN532 nfc(PN532_IRQ, PN532_RESET, &Wire);

void drawWaiting()
{
    u8g2.clearBuffer();

    u8g2.drawStr(10, 20, "PN532 Ready");
    u8g2.drawStr(10, 40, "Tap NFC Card");

    u8g2.sendBuffer();
}

void showUID(uint8_t *uid, uint8_t uidLength)
{
    char line[64];

    String uidStr = "";

    for (int i = 0; i < uidLength; i++)
    {
        if (uid[i] < 0x10)
            uidStr += "0";

        uidStr += String(uid[i], HEX);
        uidStr += " ";
    }

    uidStr.toUpperCase();

    u8g2.clearBuffer();

    u8g2.drawStr(0, 15, "Card Detected");

    snprintf(line, sizeof(line), "UID:");

    u8g2.drawStr(0, 35, line);

    u8g2.drawStr(0, 50, uidStr.c_str());

    u8g2.sendBuffer();
}

void pn532_init()
{
    delay(100);
    nfc.begin();
    delay(100);

    uint32_t versiondata = nfc.getFirmwareVersion();

    if (!versiondata)
    {
        Serial.println("PN532 not found");

        u8g2.clearBuffer();
        u8g2.drawStr(0, 20, "PN532 NOT FOUND");
        u8g2.sendBuffer();

        delay(2000);
        return;
    }

    Serial.println("PN532 initialized");

    nfc.SAMConfig();
}

void pn532_scan_loop()
{
    pn532_init();

    drawWaiting();

    while (1)
    {
        uint8_t success;
        uint8_t uid[7];
        uint8_t uidLength;

        Serial.println("Scanning...");
        success = nfc.readPassiveTargetID(
            PN532_MIFARE_ISO14443A,
            uid,
            &uidLength,
            50
        );

        if (success)
        {
            Serial.println("Card detected");

            showUID(uid, uidLength);

            delay(1000);
        }

        if (btnBack())
        {
            delay(150);
            break;
        }
    }
}
