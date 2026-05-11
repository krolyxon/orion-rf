#include <Arduino.h>
#include "../libs/ELECHOUSE_CC1101_SRC_DRV.h"
#include "cc1101.h"
#include "../config.h"
#include "SPI.h"

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
void IRAM_ATTR pulseISR()
{
    unsigned long now = micros();

    if (!capturing) return;
    if (pulseIndex >= RAW_BUF_MAX) return;

    unsigned long duration = now - lastEdgeTime;

    if (duration < 150) return;

    captureBuffer[pulseIndex++] = duration;
    lastEdgeTime = now;
}

// ===== OOK SETUP =====
void setupOOKMode()
{
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
bool initCC1101()
{
    Serial.println();
    Serial.println("===== CC1101 INIT =====");

    // ===== SPI =====
    SPI.begin(
        cc1101_SCK,
        cc1101_MISO,
        cc1101_MOSI,
        CC1101_CS
    );

    pinMode(CC1101_CS, OUTPUT);
    digitalWrite(CC1101_CS, HIGH);

    delay(100);

    // ===== GDO =====
    ELECHOUSE_cc1101.setGDO(
        CC1101_GDO0,
        -1
    );

    ELECHOUSE_cc1101.setSpiPin(
        cc1101_SCK,
        cc1101_MISO,
        cc1101_MOSI,
        CC1101_CS
    );

    // ===== DETECT =====
    Serial.println("Checking chip...");

    if (!ELECHOUSE_cc1101.getCC1101())
    {
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
void startCapture()
{
    pulseIndex = 0;
    capturing = true;
    lastEdgeTime = micros();

    attachInterrupt(
        digitalPinToInterrupt(CC1101_GDO0),
        pulseISR,
        CHANGE
    );

    Serial.println("Looking for RF... ");
}

bool isCC1101Ready() {
    return cc1101Inited;
}

void stopCapture()
{
    capturing = false;

    detachInterrupt(digitalPinToInterrupt(CC1101_GDO0));

    Serial.println("Capture stopped");
}

// ===== DEBUG PRINT =====
void printCapture()
{
    Serial.println("Captured pulses:");

    for (int i = 0; i < pulseIndex; i++)
    {
        Serial.println(captureBuffer[i]);
    }
}


// ================= REPLAY =================
void replaySignal()
{
    Serial.println();
    Serial.println("Replaying signal...");

    stopCapture();

    ELECHOUSE_cc1101.SetTx();

    pinMode(CC1101_GDO0, OUTPUT);

    for (int i = 0; i < pulseIndex; i++)
    {
        digitalWrite(
            CC1101_GDO0,
            (i % 2 == 0) ? HIGH : LOW
        );

        delayMicroseconds(captureBuffer[i]);
    }

    digitalWrite(CC1101_GDO0, LOW);

    ELECHOUSE_cc1101.SetRx();

    pinMode(CC1101_GDO0, INPUT);

    Serial.println("Replay complete");
}

