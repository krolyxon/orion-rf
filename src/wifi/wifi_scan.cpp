#include <Arduino.h>
#include <WiFi.h>
#include "../ui/display.h"
#include "../buttons.h"

#define MAX_NETWORKS 30

struct WiFiNet {
    String ssid;
    int rssi;
    int channel;
    bool encrypted;
};

static WiFiNet networks[MAX_NETWORKS];
static int networkCount = 0;
static int selectedIndex = 0;

// ===== SCAN =====
void wifi_scan_start()
{
    u8g2.clearBuffer();
    u8g2.drawStr(10, 30, "Scanning WiFi...");
    u8g2.sendBuffer();

    WiFi.mode(WIFI_STA);
    WiFi.disconnect();

    delay(100);

    int n = WiFi.scanNetworks();

    networkCount = min(n, MAX_NETWORKS);

    for (int i = 0; i < networkCount; i++)
    {
        networks[i].ssid = WiFi.SSID(i);
        networks[i].rssi = WiFi.RSSI(i);
        networks[i].channel = WiFi.channel(i);
        networks[i].encrypted = (WiFi.encryptionType(i) != WIFI_AUTH_OPEN);
    }

    selectedIndex = 0;
}

// ===== DRAW =====
void wifi_scan_draw()
{
    u8g2.clearBuffer();

    if (networkCount == 0)
    {
        u8g2.drawStr(0, 30, "No networks");
        u8g2.drawStr(0, 45, "Press BACK");
    }
    else
    {
        char counter[20];
        sprintf(counter, "%d/%d", selectedIndex + 1, networkCount);
        u8g2.setFont(u8g2_font_5x8_tr);
        u8g2.drawStr(0, 8, counter);

        u8g2.setFont(u8g2_font_6x10_tr);

        for (int i = 0; i < 3; i++)
        {
            int idx = selectedIndex + i;
            if (idx >= networkCount) break;

            int y = 22 + i * 14;

            if (i == 0)
            {
                u8g2.drawBox(0, y - 10, 128, 12);
                u8g2.setDrawColor(0);
            }

            String text = networks[idx].ssid;
            if (text.length() > 10)
                text = text.substring(0, 10) + "..";

            text += " (" + String(networks[idx].rssi) + ")";

            u8g2.drawStr(2, y, text.c_str());

            if (i == 0)
                u8g2.setDrawColor(1);
        }
    }

    u8g2.sendBuffer();
}

// ===== DETAILS =====
void wifi_drawDetails()
{
    if (networkCount == 0) return;

    WiFiNet &net = networks[selectedIndex];

    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x8_tr);

    u8g2.drawStr(0, 10, net.ssid.c_str());

    char rssi[20];
    sprintf(rssi, "RSSI: %d", net.rssi);
    u8g2.drawStr(0, 20, rssi);

    char ch[20];
    sprintf(ch, "CH: %d", net.channel);
    u8g2.drawStr(0, 30, ch);

    u8g2.drawStr(0, 40, net.encrypted ? "Secured" : "Open");

    u8g2.sendBuffer();
}

// ===== LOOP =====
void wifi_scan_loop()
{
    static uint32_t lastPress = 0;

    if (millis() - lastPress < 150)
        return;

    if (btnDown() && selectedIndex < networkCount - 1)
    {
        selectedIndex++;
        wifi_scan_draw();
        lastPress = millis();
    }
    else if (btnUp() && selectedIndex > 0)
    {
        selectedIndex--;
        wifi_scan_draw();
        lastPress = millis();
    }
    else if (btnSelect() && networkCount > 0)
    {
        wifi_drawDetails();
        delay(3000);
        wifi_scan_draw();
        lastPress = millis();
    }
//    else if (btnBack())
//    {
//        wifi_scan_start();
//        wifi_scan_draw();
//        lastPress = millis();
//    }
}
