#include <Arduino.h>
#include <BleMouse.h>
#include "badusb.h"
#include "config.h"

#include "buttons.h"
#include "display.h"

// ===== BLE MOUSE =====
// BleMouse bleMouse("Orion-RF", "Orion-RF", 100);
extern BleMouse bleMouse;

// ===== MAIN =====
void ble_mouse_run()
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_6x10_tr);
    u8g2.drawStr(10, 25, "BLE Mouse");
    u8g2.drawStr(10, 45, "Connecting...");
    u8g2.sendBuffer();

    delay(800);

    while (1)
    {
        // 🔥 EXIT FIRST (clean)
        if (btnBack()) break;

        bool connected = bleMouse.isConnected();

        int dx = 0;
        int dy = 0;

        if (connected)
        {
            if (!digitalRead(BTN_UP))    dy = -6;
            if (!digitalRead(BTN_DOWN))  dy = 6;
            if (!digitalRead(BTN_LEFT))  dx = -6;
            if (!digitalRead(BTN_RIGHT)) dx = 6;

            if (dx || dy)
                bleMouse.move(dx, dy);

            // ✅ single click (not spam)
            static bool lastSelect = false;
            bool currentSelect = !digitalRead(BTN_SELECT);

            if (currentSelect && !lastSelect)
                bleMouse.click(MOUSE_LEFT);

            lastSelect = currentSelect;
        }

        // ===== UI =====
        u8g2.clearBuffer();
        u8g2.setFont(u8g2_font_6x10_tr);

        u8g2.drawStr(10, 20, "BLE Mouse");

        if (connected)
            u8g2.drawStr(10, 35, "Connected");
        else
            u8g2.drawStr(10, 35, "Waiting");

        u8g2.drawStr(10, 55, "BACK = Exit");

        u8g2.sendBuffer();

        delay(10);  // important for BLE stability
    }
}



