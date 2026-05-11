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
#include "ble_mouse.h"
#include "sysinfo.h"
#include "BleMouse.h"
#include "nfc.h"

// ================= MENU DATA =================
extern BleMouse bleMouse;

// Root menu
const char *mainMenuItems[] = {
    "BadUSB",
    "RF Capture",
    "NRF Tools",
    "BLE Scan",
    "Wifi Scan",
    "Wifi Analyzer",
    "System Info",
    "Device Check",
    "Restart",
    "Ble Mouse",
    "NFC Tools"
};

Menu mainMenu = {mainMenuItems, sizeof(mainMenuItems) / sizeof(mainMenuItems[0])};


// NRF Tools menu
const char *nrfToolsItems[] = {
    "BLE Jammer",
    "Bluetooth Jammer",
    "Wifi Jammer",
    "USB Wireless",
    "Video TX",
    "Zigbee",
    "RC"

};

Menu nrfToolsMenu = {nrfToolsItems, sizeof(nrfToolsItems) / sizeof(nrfToolsItems[0])};


// BadUSB submenu
const char *badusbItems[] = {"DEMO",
                             "KEYBOARD",
                             "HID SCRIPT",
                             "Open Notepad",
                             "Open CMD",
                             "Show IP",
                             "Shutdown",
                             "RickRoll",
                             "Create Admin",
                             "Disable Defender",
                             "Open YouTube",
                             "Lock PC",
                             "Fake Update",
                             "Endless Notepad",
                             "Fake BSOD",
                             "Flip Screen",
                             "Matrix Effect",
                             "I'm Watching U",
                             "Open Google",
                             "Open telegram",
                             "Play Alarm Sound",
                             "Endless CMD",
                             "Type Gibberish",
                             "Spam CAPSLOCK",
                             "Open Calc",
                             "Auto 'Hacked!'",
                             "Turn Off Monitor",
                             "Open RegEdit",
                             "Kill Explorer",
                             "Flash Screen",
                             "Rename Desktop",
                             "Toggle WiFi",
                             "Auto Screenshot",
                             "Spam Emojis",
                             "Open Ctrl Panel",
                             "Troll Wallpaper",
                             "Open MS Paint",
                             "Tab Switcher"};


Menu badusbMenu = {badusbItems, sizeof(badusbItems) / sizeof(badusbItems[0])};

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
            delay(5000);
            replaySignal();
            break;

            case 2:
                // startNRFJammer();
                //startBleJammer();
                //startBluetoothJammer();
                currentMenu = &nrfToolsMenu;
                menuIndex = 0;
                menuOffset = 0;
                break;

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
            {
                  // wait for button release
                 delay(200);

                while (btnSelect())
                    delay(10);
                bool confirm = false;

                while (1)
                {
                    u8g2.clearBuffer();
                    u8g2.setFont(u8g2_font_6x13_tr);

                    u8g2.drawStr(18, 18, "Restart Device?");

                    if (confirm)
                    {
                        u8g2.drawBox(10, 35, 45, 15);
                        u8g2.setDrawColor(0);
                        u8g2.drawStr(20, 47, "YES");
                        u8g2.setDrawColor(1);

                        u8g2.drawStr(75, 47, "NO");
                    }
                    else
                    {
                        u8g2.drawStr(20, 47, "YES");

                        u8g2.drawBox(65, 35, 45, 15);
                        u8g2.setDrawColor(0);
                        u8g2.drawStr(78, 47, "NO");
                        u8g2.setDrawColor(1);
                    }

                    u8g2.sendBuffer();

                    if (btnLeft() || btnUp())
                    {
                        confirm = true;
                        delay(150);
                    }

                    if (btnRight() || btnDown())
                    {
                        confirm = false;
                        delay(150);
                    }

                    if (btnSelect())
                    {
                        delay(150);

                        if (confirm)
                        {
                            u8g2.clearBuffer();
                            u8g2.drawStr(28, 30, "Restarting...");
                            u8g2.sendBuffer();

                            delay(1000);
                            ESP.restart();
                        }
                        else
                        {
                            break;
                        }
                    }

                    if (btnBack())
                    {
                        delay(150);
                        break;
                    }
                }
            }
        break;

        case 9:
                  // Begin Ble mouse
                  bleMouse.begin();
                  ble_mouse_run();
                    break;
        case 10:
                    pn532_scan_loop();
                    break;
        }
    }
    else if (currentMenu == &badusbMenu)
    {
        badUSBMenu(menuIndex);
    } else if (currentMenu == &nrfToolsMenu) {
        NRFToolsMenu(menuIndex);
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
