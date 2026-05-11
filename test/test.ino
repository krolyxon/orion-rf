#include <Arduino.h>
#include <SPI.h>
#include <RF24.h>
#include <U8g2lib.h>
#include <WiFi.h>
#include "esp_bt.h"

// ================= NRF24 =================
#define CE1_PIN 10
#define CSN1_PIN 11

#define CE2_PIN 12
#define CSN2_PIN 13

#define NRF_SCK 18
#define NRF_MISO 16
#define NRF_MOSI 17

// ================= OLED ==================
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9

// ================= Buttons ===============
#define BTN_UP 4

// =========================================

// Third radio optional
#define CE3_PIN 46
#define CSN3_PIN 48

SPIClass *NRF_SPI = nullptr;

// RF24 radios
RF24 RadioA(CE1_PIN, CSN1_PIN);
RF24 RadioB(CE2_PIN, CSN2_PIN);

// OLED
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(
    U8G2_R0,
    U8X8_PIN_NONE,
    OLED_SCL_PIN,
    OLED_SDA_PIN
);

enum OperationMode {
    DEACTIVE_MODE,
    BLE_MODULE,
    Bluetooth_MODULE
};

OperationMode currentMode = DEACTIVE_MODE;

// BLE advertising channels
const byte ble_channels[] = {2, 26, 80};

// Bluetooth hopping channels
//const byte bluetooth_channels[] = {
//    32, 34, 46, 48, 50, 52,
//    0, 1, 2, 4, 6, 8,
//    22, 24, 26, 28, 30,
//    74, 76, 78, 80
//};

const byte bluetooth_channels[] = {
    12, 17, 22, 27, 32,
    37, 42, 47, 52, 57,
    62, 67, 72
};

volatile bool modeChangeRequested = false;

unsigned long lastButtonPressTime = 0;
const unsigned long debounceDelay = 500;

// =========================================
// NRF CONFIG
// =========================================

void configureNrf(RF24 &radio) {

    radio.begin(NRF_SPI);

    radio.setAutoAck(false);
    radio.stopListening();

    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX);
    radio.setDataRate(RF24_2MBPS);

    radio.disableCRC();
}

// =========================================

void IRAM_ATTR handleButtonPress() {

    unsigned long currentTime = millis();

    if (currentTime - lastButtonPressTime > debounceDelay) {
        modeChangeRequested = true;
        lastButtonPressTime = currentTime;
    }
}

// =========================================

void configureRadio(RF24 &radio) {

    configureNrf(radio);

    radio.startConstCarrier(
        RF24_PA_MAX,
        40
    );
}

// =========================================

void initializeRadios() {

    if (currentMode == DEACTIVE_MODE) {

        RadioA.powerDown();
        RadioB.powerDown();

        return;
    }

    configureRadio(RadioA);
    configureRadio(RadioB);
}

// =========================================

void updateOLED() {

    u8g2.clearBuffer();

    u8g2.setFont(u8g2_font_6x10_tr);

    u8g2.setCursor(0, 12);
    u8g2.print("Mode:");

    u8g2.setCursor(50, 12);

    if (currentMode == BLE_MODULE) {
        u8g2.print("BLE");
    }
    else if (currentMode == Bluetooth_MODULE) {
        u8g2.print("BT");
    }
    else {
        u8g2.print("OFF");
    }

    u8g2.setCursor(0, 30);
    u8g2.print("R1:");
    u8g2.print(RadioA.isChipConnected());

    u8g2.setCursor(0, 45);
    u8g2.print("R2:");
    u8g2.print(RadioB.isChipConnected());

    u8g2.setCursor(0, 60);
    u8g2.print("R3:");

    u8g2.sendBuffer();
}

// =========================================

void checkModeChange() {

    if (!modeChangeRequested)
        return;

    modeChangeRequested = false;

    currentMode =
        static_cast<OperationMode>(
            (currentMode + 1) % 3
        );

    initializeRadios();

    updateOLED();
}

// =========================================
// SETUP
// =========================================

void setup() {

    Serial.begin(115200);

    // Disable ESP32 radios
    esp_bt_controller_deinit();

    WiFi.mode(WIFI_OFF);
    btStop();

    // SPI
    NRF_SPI = new SPIClass(FSPI);

    NRF_SPI->begin(
        NRF_SCK,
        NRF_MISO,
        NRF_MOSI
    );

    // OLED
    u8g2.begin();

    // Button
    pinMode(BTN_UP, INPUT_PULLUP);

    attachInterrupt(
        digitalPinToInterrupt(BTN_UP),
        handleButtonPress,
        FALLING
    );

    initializeRadios();

    updateOLED();

    Serial.println("Jammer initialized");
}

// =========================================
// LOOP
// =========================================

void loop() {

    checkModeChange();

    if (currentMode == BLE_MODULE) {

        byte channel =
            ble_channels[
                random(
                    sizeof(ble_channels)
                )
            ];

        RadioA.setChannel(channel);
        RadioB.setChannel(channel);
    }

    else if (currentMode == Bluetooth_MODULE) {

        byte channel =
            bluetooth_channels[
                random(
                    sizeof(bluetooth_channels)
                )
            ];

        RadioA.setChannel(channel);
        RadioB.setChannel(channel);
    }

    delay(5);
}
