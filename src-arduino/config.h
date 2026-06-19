#pragma once

// ================= NRF24 =================
#define CE1_PIN 10
#define CSN1_PIN 11

#define CE2_PIN 12
#define CSN2_PIN 13

#define NRF_SCK 18
#define NRF_MISO 16
#define NRF_MOSI 17

// ================== OLED ===================
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9

/////////////////cc1101 vars//////////////
// CC1101 via FSPI
#define cc1101_SCK 15
#define cc1101_MISO 41
#define cc1101_MOSI 35

//////////////cc1101(1)//////////
#define CC1101_CS 40
#define CC1101_GDO0 39
#define CC1101_GDO2 42

// SD Card via HSPI
// #define SD_SCK 14
// #define SD_MISO 39
// #define SD_MOSI 38
// #define SD_CS 37

// =================== Buttons ====================
#define BTN_UP 4
#define BTN_DOWN 5
#define BTN_SELECT 6
#define BTN_BACK 7
#define BTN_LEFT 1
#define BTN_RIGHT 2
