#include "../libs/ELECHOUSE_CC1101_SRC_DRV.h"
#include <Arduino.h>
#include <RF24.h>
#include <SPI.h>
#include <Wire.h>

#include "../config.h"
#include "../ui/display.h"
#include "buttons.h"

// ===== EXTERNALS =====
extern RF24 radio1;
extern RF24 radio2;
extern SPIClass *RADIO_SPI;
extern U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2;

// ===== RESULTS =====
struct DeviceStatus {
  bool nrf1 = false;
  bool nrf2 = false;
  bool cc1101 = false;
  bool oled = true;
};

// ===== NRF CHECK =====
// bool checkNRF(RF24 &radio)
//{
//    // safer: only init if needed
//    if (!radio.isChipConnected()) {
//        if (!radio.begin(RADIO_SPI))
//            return false;
//    }
//
//    return radio.isChipConnected();
//}

bool checkNRF(RF24 &radio) {
  radio.powerDown();
  delay(5);

  if (!radio.begin(RADIO_SPI))
    return false;

  delay(5);

  return radio.isChipConnected();
}

// ===== CC1101 CHECK =====
bool checkCC1101(uint8_t csPin) {
  ELECHOUSE_cc1101.setSpiPin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, csPin);

  delay(5);

  return ELECHOUSE_cc1101.getCC1101();
}

// ===== DRAW =====
#define MAX_ITEMS 4
#define VISIBLE_ROWS 5

const char *labels[MAX_ITEMS] = {"NRF1", "NRF2", "CC1101", "OLED"};

bool values[MAX_ITEMS];

int selectedIndex = 0;
int offset = 0;

void drawStatus(DeviceStatus &s) {
  values[0] = s.nrf1;
  values[1] = s.nrf2;
  values[2] = s.cc1101;
  values[3] = s.oled;

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);

  // scrolling logic
  if (selectedIndex < offset)
    offset = selectedIndex;

  if (selectedIndex >= offset + VISIBLE_ROWS)
    offset = selectedIndex - VISIBLE_ROWS + 1;

  for (int i = 0; i < VISIBLE_ROWS; i++) {
    int item = offset + i;
    if (item >= MAX_ITEMS)
      break;

    int y = 12 + i * 10;

    if (item == selectedIndex) {
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

// ===== MAIN =====
void device_check_run() {
  DeviceStatus status;

  Serial.println("Running device diagnostics...");

  // NRF
  status.nrf1 = checkNRF(radio1);
  status.nrf2 = checkNRF(radio2);

  // CC1101
  status.cc1101 = checkCC1101(CC1101_CS);
  // status.cc1101 = true;

  drawStatus(status);

  Serial.println("Diagnostics complete");

  while (1) {
    drawStatus(status);

    if (btnUp()) {
      selectedIndex--;
      if (selectedIndex < 0)
        selectedIndex = MAX_ITEMS - 1;
      delay(150);
    }

    if (btnDown()) {
      selectedIndex++;
      if (selectedIndex >= MAX_ITEMS)
        selectedIndex = 0;
      delay(150);
    }

    if (btnBack()) {
      delay(150);
      break;
    }
  }
}
