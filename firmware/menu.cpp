#include <Arduino.h>
#include "menu.h"
#include "display.h"
#include "buttons.h"
#include "badusb.h"
#include "nrf24.h"
#include "cc1101.h"
#include "blescanner.h"
#include "wifi_scan.h"
#include "wifi_analyzer.h"
#include "device_check.h"

// ================= FEATURE HANDLERS =================
void runSystemInfoFeature();
void runRFCaptureFeature();
void runBLEScanFeature();



// ================= MENU DATA =================

// Root menu
const char *mainMenuItems[] = {
    "BadUSB",
    "RF Capture",
    "NRF Jammer",
    "BLE Scan",
    "Wifi Scan",
    "Wifi Analyzer",
    "System Info",
    "Device Check",
    "Restart"
};

Menu mainMenu = {mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0])};

// BadUSB submenu
const char *badusbItems[] = {
    "Demo",
    "Open CMD",
    "Rickroll"};

Menu badusbMenu = {badusbItems, 3};

// ================= MENU STATE =================

Menu *currentMenu = &mainMenu;

int menuIndex = 0;
int menuOffset = 0;

#define MENU_VISIBLE_ROWS 4

bool insideFeature = false;

// ================= DRAW =================

void drawMenu()
{
    u8g2.clearBuffer();

    // scroll handling
    if (menuIndex < menuOffset)
        menuOffset = menuIndex;

    if (menuIndex >= menuOffset + MENU_VISIBLE_ROWS)
        menuOffset = menuIndex - MENU_VISIBLE_ROWS + 1;

    for (int i = 0; i < MENU_VISIBLE_ROWS; i++)
    {
        int item = menuOffset + i;

        if (item >= currentMenu->size)
            break;

        if (item == menuIndex)
            u8g2.drawStr(0, 14 + i * 14, ">");

        u8g2.drawStr(10, 14 + i * 14, currentMenu->items[item]);
    }

    // scroll indicators
    if (menuOffset > 0)
        u8g2.drawStr(118, 10, "^");

    if (menuOffset + MENU_VISIBLE_ROWS < currentMenu->size)
        u8g2.drawStr(118, 62, "v");

    u8g2.sendBuffer();
}

// ================= FEATURE EXECUTION =================

void launchFeature()
{
    insideFeature = true;

    if (currentMenu == &mainMenu)
    {
        switch (menuIndex)
        {
            case 0: // BadUSB → enter submenu
                currentMenu = &badusbMenu;
                menuIndex = 0;
                menuOffset = 0;
                break;

            case 1:
            if (!isCC1101Ready()) {
            if (!initCC1101()) {
            Serial.println("CC1101 failed");
            return;
            }
            }

            Serial.println("Ready to capture...");

            startCapture();
            delay(5000);
            stopCapture();
            printCapture();
            break;
            case 2:
                startNRFJammer();
                break;

            case 3:
                ble_scan();
                ble_drawMenu();
                while (1) {
                    ble_loop();
                    if (btnBack())
                        break;
                }
                break;

            case 4:
            {
            // Start scan once
            wifi_scan_start();
            wifi_scan_draw();

            while (1) {
                wifi_scan_loop();
                // EXIT condition handled ONLY here
                if (btnBack()) {
                    delay(150); // debounce
                    break;
                }
            }
            break;
            }
            case 5:
                {

                wifi_analyzer_start();

                bool prevBack = false;
                while (1)
                {
                    wifi_analyzer_loop();
                    bool nowBack = btnBack();
                    if (nowBack && !prevBack)
                    {
                        delay(150);
                        break;
                    }
                    prevBack = nowBack;
                }
                break;
                }

            case 6:
                runSystemInfoFeature();
                break;
            case 7:
                device_check_run();
                break;
            case 8:
                 u8g2.clearBuffer();
                 u8g2.setFont(u8g2_font_6x13_tr);
                 u8g2.drawStr(30, 30, "Restarting...");
                 u8g2.sendBuffer();
                 delay(1000);
                 ESP.restart();
                break;
        }
    }
    else if (currentMenu == &badusbMenu)
    {
        switch (menuIndex)
        {
            case 0:
                runBadUSBDemo();
                break;

            case 1:
                Serial.println("Open CMD payload");
                runBadUSBOpenCMD();
                break;

            case 2:
                Serial.println("Rickroll payload");
                runBadUSBRickroll();
                break;
        }
    }

    insideFeature = false;

    drawMenu();
}

// ================= INIT =================

void menuInit()
{
    currentMenu = &mainMenu;
    menuIndex = 0;
    menuOffset = 0;

    drawMenu();
}

// ================= LOOP =================

void menuLoop()
{
    static uint32_t lastPress = 0;

    if (insideFeature)
        return;

    if (millis() - lastPress < 150)
        return;

    if (btnUp())
    {
        menuIndex--;

        if (menuIndex < 0)
            menuIndex = currentMenu->size - 1;

        drawMenu();
        lastPress = millis();
    }

    else if (btnDown())
    {
        menuIndex++;

        if (menuIndex >= currentMenu->size)
            menuIndex = 0;

        drawMenu();
        lastPress = millis();
    }

    else if (btnSelect())
    {
        launchFeature();
        lastPress = millis();
    }

    else if (btnBack())
    {
        if (currentMenu != &mainMenu)
        {
            currentMenu = &mainMenu;
            menuIndex = 0;
            menuOffset = 0;

            drawMenu();
        }

        lastPress = millis();
    }
}
