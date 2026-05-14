#include "cc1101.h"
#include "../config.h"
#include "../libs/ELECHOUSE_CC1101_SRC_DRV.h"
#include "../ui/display.h"
#include "../utils/buttons.h"
#include "SPI.h"
#include <Arduino.h>

// ===== CONFIG =====
#define RAW_BUF_MAX 512

// ===== STATE =====
static bool cc1101Inited = false;

// ===== CAPTURE STATE =====
volatile unsigned long captureBuffer[RAW_BUF_MAX];
volatile int pulseIndex = 0;
volatile unsigned long lastEdgeTime = 0;
volatile bool capturing = false;

// ===== RF SETTINGS =====
float currentFreq = 433.92;
float dataRate = 3.79372;
float deviation = 0.0;
float rxBW = 325.0;
int powerLevel = 10;

// ===== ISR =====
void IRAM_ATTR pulseISR() {
  unsigned long now = micros();

  if (!capturing)
    return;
  if (pulseIndex >= RAW_BUF_MAX)
    return;

  unsigned long duration = now - lastEdgeTime;

  if (duration < 150)
    return;

  captureBuffer[pulseIndex++] = duration;
  lastEdgeTime = now;
}

// ===== OOK SETUP =====
void setupOOKMode() {
  ELECHOUSE_cc1101.SetRx();
  ELECHOUSE_cc1101.setMHZ(currentFreq);

  ELECHOUSE_cc1101.setModulation(2); // ASK/OOK
  ELECHOUSE_cc1101.setDRate(dataRate);
  ELECHOUSE_cc1101.setDeviation(0);
  ELECHOUSE_cc1101.setRxBW(rxBW);
  ELECHOUSE_cc1101.setSyncMode(0);
  ELECHOUSE_cc1101.setPA(powerLevel);
}

// ===== INIT (LAZY, SAFE) =====
// ================= CC1101 INIT =================
bool initCC1101() {
  Serial.println();
  Serial.println("===== CC1101 INIT =====");

  // ===== SPI =====
  SPI.begin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, CC1101_CS);

  pinMode(CC1101_CS, OUTPUT);
  digitalWrite(CC1101_CS, HIGH);

  delay(100);

  // ===== GDO =====
  ELECHOUSE_cc1101.setGDO(CC1101_GDO0, -1);

  ELECHOUSE_cc1101.setSpiPin(cc1101_SCK, cc1101_MISO, cc1101_MOSI, CC1101_CS);

  // ===== DETECT =====
  Serial.println("Checking chip...");

  if (!ELECHOUSE_cc1101.getCC1101()) {
    Serial.println("❌ CC1101 NOT FOUND");
    return false;
  }

  Serial.println("✅ CC1101 FOUND");

  // ===== IMPORTANT =====
  // DO NOT CALL Init()
  // it freezes on some ESP32-S3 setups

  // ===== MANUAL CONFIG =====
  ELECHOUSE_cc1101.setMHZ(currentFreq);

  // 2 = ASK/OOK
  ELECHOUSE_cc1101.setModulation(2);

  ELECHOUSE_cc1101.setDRate(dataRate);

  ELECHOUSE_cc1101.setRxBW(rxBW);

  ELECHOUSE_cc1101.setDeviation(0);

  // disable sync requirement
  ELECHOUSE_cc1101.setSyncMode(0);

  ELECHOUSE_cc1101.setPA(powerLevel);

  // async serial mode
  ELECHOUSE_cc1101.setCCMode(0);

  // enter RX
  ELECHOUSE_cc1101.SetRx();

  pinMode(CC1101_GDO0, INPUT);

  Serial.println("✅ RX MODE READY");

  cc1101Inited = true;
  return true;
}
// ===== CAPTURE CONTROL =====
void startCapture() {
  pulseIndex = 0;
  capturing = true;
  lastEdgeTime = micros();

  attachInterrupt(digitalPinToInterrupt(CC1101_GDO0), pulseISR, CHANGE);

  Serial.println("Looking for RF... ");
}

bool isCC1101Ready() { return cc1101Inited; }

void stopCapture() {
  capturing = false;

  detachInterrupt(digitalPinToInterrupt(CC1101_GDO0));

  Serial.println("Capture stopped");
}

// ===== DEBUG PRINT =====
void printCapture() {
  Serial.println("Captured pulses:");

  for (int i = 0; i < pulseIndex; i++) {
    Serial.println(captureBuffer[i]);
  }
}

// ================= REPLAY =================
void replaySignal() {
  Serial.println();
  Serial.println("Replaying signal...");

  stopCapture();

  ELECHOUSE_cc1101.SetTx();

  pinMode(CC1101_GDO0, OUTPUT);

  for (int i = 0; i < pulseIndex; i++) {
    digitalWrite(CC1101_GDO0, (i % 2 == 0) ? HIGH : LOW);

    delayMicroseconds(captureBuffer[i]);
  }

  digitalWrite(CC1101_GDO0, LOW);

  ELECHOUSE_cc1101.SetRx();

  pinMode(CC1101_GDO0, INPUT);

  Serial.println("Replay complete");
}

void captureAndDisplay() {
  if (!cc1101Inited) {
    if (!initCC1101()) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(0, 20, "CC1101 Failed");
      u8g2.sendBuffer();
      return;
    }
  }

  startCapture();

  u8g2.clearBuffer();
  u8g2.setFont(u8g2_font_6x10_tr);
  u8g2.drawStr(0, 12, "CC1101 Capture");
  u8g2.drawStr(0, 28, "Waiting RF...");
  u8g2.drawStr(0, 60, "BACK = Exit");
  u8g2.sendBuffer();

  unsigned long lastSignal = millis();
  int lastPulseCount = 0;

  while (true) {
    // signal detected
    if (pulseIndex > lastPulseCount) {
      lastPulseCount = pulseIndex;
      lastSignal = millis();
    }

    // update OLED
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    u8g2.drawStr(0, 12, "CC1101 Capture");

    u8g2.setCursor(0, 28);
    u8g2.print("Pulses: ");
    u8g2.print(pulseIndex);

    u8g2.setCursor(0, 42);
    u8g2.print("Freq: ");
    u8g2.print(currentFreq);
    u8g2.print(" MHz");

    if (pulseIndex > 0) {
      u8g2.drawStr(0, 54, "Signal Detected");
    } else {
      u8g2.drawStr(0, 54, "Waiting...");
    }

    u8g2.sendBuffer();

    // auto print once capture stabilizes
    if (pulseIndex > 20 && (millis() - lastSignal > 1500)) {
      stopCapture();

      Serial.println();
      Serial.println("===== RF CAPTURE =====");

      for (int i = 0; i < pulseIndex; i++) {
        Serial.print(captureBuffer[i]);
        Serial.print(", ");
      }

      Serial.println();

      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(0, 15, "Capture Complete");

      u8g2.setCursor(0, 35);
    }
  }
}

void handleMenu() {
  if (!isCC1101Ready()) {
    if (!initCC1101()) {
      u8g2.clearBuffer();
      u8g2.setFont(u8g2_font_6x10_tr);
      u8g2.drawStr(0, 20, "CC1101 Failed");
      u8g2.sendBuffer();
      delay(1500);
      return;
    }
  }

  pulseIndex = 0;

  startCapture();

  // ===== CAPTURE FOR 5 SEC =====
  unsigned long start = millis();

  while (millis() - start < 5000) {
    noInterrupts();
    int count = pulseIndex;
    interrupts();

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);

    u8g2.drawStr(0, 12, "RF Capturing...");

    u8g2.setCursor(0, 30);
    u8g2.print("Pulses: ");
    u8g2.print(count);

    u8g2.sendBuffer();

    delay(50);
  }

  stopCapture();

  // ===== DISPLAY CAPTURE BUFFER =====
  int scroll = 0;

  while (1) {
    noInterrupts();
    int count = pulseIndex;
    interrupts();

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x8_tr);

    u8g2.drawStr(0, 8, "Captured Buffer");

    // display 6 lines
    for (int i = 0; i < 6; i++) {
      int idx = scroll + i;

      if (idx >= count)
        break;

      noInterrupts();
      unsigned long val = captureBuffer[idx];
      interrupts();

      char buf[32];

      snprintf(buf, sizeof(buf), "%03d: %lu", idx, val);

      u8g2.drawStr(0, 20 + (i * 8), buf);
    }

    u8g2.sendBuffer();

    // scroll down
    if (btnDown()) {
      if (scroll < count - 1)
        scroll++;

      delay(120);
    }

    // scroll up
    if (btnUp()) {
      if (scroll > 0)
        scroll--;

      delay(120);
    }

    // replay
    if (btnSelect()) {
      replaySignal();
      delay(300);
    }

    // exit
    if (btnBack()) {
      delay(150);
      return;
    }

    delay(20);
  }
}
