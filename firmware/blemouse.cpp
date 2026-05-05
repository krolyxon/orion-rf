#include <Arduino.h>
#include <BleMouse.h>
#include "config.h"

#include "buttons.h"
#include "display.h"

// ===== BLE MOUSE =====
BleMouse bleMouse("Orion-RF", "Orion-RF", 100);

// ===== MAIN =====
void ble_mouse_run()
{
    bleMouse.begin();

    // simple screen
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(10, 25, "BLE Mouse");
    u8g2.drawStr(10, 45, "Connecting...");
    u8g2.sendBuffer();

    delay(1000);

    while (1)
    {
        if (btnBack()) break;

        bool connected = bleMouse.isConnected();

        int dx = 0;
        int dy = 0;

        if (connected)
        {
            if (!digitalRead(BTN_UP))    dy = -8;
            if (!digitalRead(BTN_DOWN))  dy = 8;
            if (!digitalRead(BTN_LEFT))  dx = -8;
            if (!digitalRead(BTN_RIGHT)) dx = 8;

            if (dx != 0 || dy != 0)
                bleMouse.move(dx, dy);

            if (!digitalRead(BTN_SELECT))
                bleMouse.click(MOUSE_LEFT);

            if (!digitalRead(BTN_BACK))
                bleMouse.click(MOUSE_RIGHT);
        }

        // ===== UI =====
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);

        u8g2.drawStr(10, 20, "BLE Mouse");

        if (connected)
            u8g2.drawStr(10, 35, "Status: Connected");
        else
            u8g2.drawStr(10, 35, "Status: Waiting");

        u8g2.drawStr(10, 55, "BACK = Exit");

        u8g2.sendBuffer();

        delay(30);
    }
}
