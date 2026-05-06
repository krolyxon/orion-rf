#include <Arduino.h>
#include <USB.h>
#include <USBHIDKeyboard.h>

#include <BLEDevice.h>
#include <BLEScan.h>
#include <BleMouse.h>

#include <RF24.h>
#include <nRF24L01.h>

#include "ELECHOUSE_CC1101_SRC_DRV.h"

#include <WiFi.h>
#include <esp_wifi.h>

#include <esp_chip_info.h>
#include <esp_heap_caps.h>
#include <esp_system.h>
#include <SPI.h>


#include "display.h"
#include "buttons.h"
#include "menu.h"

#include "config.h"
#include "cc1101.h"

// ================= USB HID =================
USBHIDKeyboard Keyboard;

// ===== BLE MOUSE =====
BleMouse bleMouse("Orion-RF", "Orion-RF", 100);


RF24 radio1(CE1_PIN, CSN1_PIN);
RF24 radio2(CE2_PIN, CSN2_PIN);
SPIClass *RADIO_SPI;


void deactivateNRF1() {
  digitalWrite(CSN1_PIN, HIGH);
  digitalWrite(CE1_PIN, LOW);
}

void deactivateNRF2() {
  digitalWrite(CSN2_PIN, HIGH);
  digitalWrite(CE2_PIN, LOW);
}


// ================= SYSTEM INFO =================
void printSystemUsage()
{
    esp_chip_info_t chip_info;

    esp_chip_info(&chip_info);

    Serial.printf("CPU cores: %d\n", chip_info.cores);

    Serial.printf(
        "Free heap: %d bytes\n",
        heap_caps_get_free_size(MALLOC_CAP_DEFAULT)
    );

    Serial.printf(
        "PSRAM free: %d bytes\n",
        heap_caps_get_free_size(MALLOC_CAP_SPIRAM)
    );
}

void showSplash()
{
    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_logisoso20_tr); // big font
    u8g2.drawStr(10, 40, "Orion-RF");

    u8g2.setFont(u8g2_font_5x8_tr); // small subtitle
    u8g2.drawStr(25, 58, "Initializing...");

    u8g2.sendBuffer();

    delay(1500); // 1.5 sec
}


// ================= SETUP =================
void setup()
{
    Serial.begin(115200);

    displayInit();
    showSplash();

    buttonsInit();
    menuInit();

    delay(1500);

    USB.begin();
    Keyboard.begin();



    // NRF SPI safety
    //pinMode(CSN1_PIN, OUTPUT);
    //digitalWrite(CSN1_PIN, HIGH);

    //pinMode(CSN2_PIN, OUTPUT);
    //digitalWrite(CSN2_PIN, HIGH);
    deactivateNRF1();
    deactivateNRF2();

    RADIO_SPI = new SPIClass(FSPI);
    RADIO_SPI->begin(NRF_SCK, NRF_MISO, NRF_MOSI);

    printSystemUsage();

    Serial.println("SYSTEM READY");
}

// ================= LOOP =================
void loop()
{
   menuLoop();
}
