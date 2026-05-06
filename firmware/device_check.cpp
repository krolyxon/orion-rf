#include <Arduino.h>
#include <SPI.h>
#include <Wire.h>
#include <RF24.h>
#include "ELECHOUSE_CC1101_SRC_DRV.h"

#include "display.h"
#include "buttons.h"
#include "config.h"

// ===== EXTERNALS =====
extern RF24 radio1;
extern RF24 radio2;
extern SPIClass *RADIO_SPI;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// ===== RESULTS =====
struct DeviceStatus {
    bool nrf_link = false;
    bool nrf1 = false;
    bool nrf2 = false;
    bool cc1101 = false;
    bool oled = true;
    bool buttons = false;
};

// ===== NRF CHECK =====
bool checkNRF(RF24 &radio)
{
    // safer: only init if needed
    if (!radio.isChipConnected()) {
        if (!radio.begin(RADIO_SPI))
            return false;
    }

    return radio.isChipConnected();
}

// ===== CC1101 CHECK =====
bool checkCC1101()
{
    ELECHOUSE_cc1101.setSpiPin(
        cc1101_SCK,
        cc1101_MISO,
        cc1101_MOSI,
        CC1101_CS
    );

    delay(5); // important stabilization

    return ELECHOUSE_cc1101.getCC1101();
}

// ===== BUTTON CHECK =====
bool checkButtons()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(0, 20, "Press any button...");
    u8g2.sendBuffer();

    unsigned long start = millis();

    while (millis() - start < 2000)
    {
        if (!digitalRead(BTN_UP) ||
            !digitalRead(BTN_DOWN) ||
            !digitalRead(BTN_SELECT) ||
            !digitalRead(BTN_BACK))
        {
            return true;
        }
    }

    return false;
}

// ===== DRAW =====
#define MAX_ITEMS 6
#define VISIBLE_ROWS 5

const char* labels[MAX_ITEMS] = {
    "NRF1",
    "NRF2",
    "LINK",
    "CC1101",
    "BUTTONS",
    "OLED"
};

bool values[MAX_ITEMS];

int selectedIndex = 0;
int offset = 0;

void drawStatus(DeviceStatus &s)
{
    values[0] = s.nrf1;
    values[1] = s.nrf2;
    values[2] = s.nrf_link;
    values[3] = s.cc1101;
    values[4] = s.buttons;
    values[5] = s.oled;

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    // scrolling logic
    if (selectedIndex < offset)
        offset = selectedIndex;

    if (selectedIndex >= offset + VISIBLE_ROWS)
        offset = selectedIndex - VISIBLE_ROWS + 1;

    for (int i = 0; i < VISIBLE_ROWS; i++)
    {
        int item = offset + i;
        if (item >= MAX_ITEMS) break;

        int y = 12 + i * 10;

        if (item == selectedIndex)
        {
            u8g2.drawBox(0, y - 9, 128, 10);
            u8g2.setDrawColor(0);
        }

        u8g2.drawStr(2, y, labels[item]);

        if (values[item])
            u8g2.drawStr(80, y, "OK");
        else
            u8g2.drawStr(80, y, "FAIL");

        if (item == selectedIndex)
            u8g2.setDrawColor(1);
    }

    u8g2.sendBuffer();
}

bool testNRFLink()
{
    const byte address[6] = "00001";
    uint8_t payload = 0xAB;
    uint8_t received = 0;

    // --- init radios ---
    if (!radio1.begin(RADIO_SPI)) return false;
    if (!radio2.begin(RADIO_SPI)) return false;

    radio1.setPALevel(RF24_PA_LOW);
    radio2.setPALevel(RF24_PA_LOW);

    radio1.setDataRate(RF24_1MBPS);
    radio2.setDataRate(RF24_1MBPS);

    radio1.setChannel(100);
    radio2.setChannel(100);

    radio1.setAutoAck(false);
    radio2.setAutoAck(false);

    // --- configure pipes ---
    radio1.openWritingPipe(address);
    radio2.openReadingPipe(0, address);

    radio2.startListening();

    delay(50);

    // --- send ---
    radio1.stopListening();
    bool sent = radio1.write(&payload, sizeof(payload));

    if (!sent) return false;

    // --- receive ---
    unsigned long start = millis();

    while (millis() - start < 200)
    {
        if (radio2.available())
        {
            radio2.read(&received, sizeof(received));
            return (received == payload);
        }
    }

    return false;
}

// ===== MAIN =====
void device_check_run()
{
    DeviceStatus status;

    Serial.println("Running device diagnostics...");

    // NRF
    // NRF link test
    status.nrf_link = testNRFLink();
    status.nrf1 = checkNRF(radio1);
    status.nrf2 = checkNRF(radio2);

    // CC1101
    status.cc1101 = checkCC1101();

    // Buttons
    status.buttons = checkButtons();

    drawStatus(status);

    Serial.println("Diagnostics complete");

    while (1)
{
    drawStatus(status);

    if (btnUp())
    {
        selectedIndex--;
        if (selectedIndex < 0) selectedIndex = MAX_ITEMS - 1;
        delay(150);
    }

    if (btnDown())
    {
        selectedIndex++;
        if (selectedIndex >= MAX_ITEMS) selectedIndex = 0;
        delay(150);
    }

    if (btnBack())
    {
        delay(150);
        break;
    }
}}



