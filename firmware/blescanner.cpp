#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEScan.h>
#include <BLEAdvertisedDevice.h>
#include <vector>

#include "display.h"
#include "buttons.h"
#include "config.h"

// ===== DEVICE STRUCT =====
struct BLEDeviceInfo {
    String name;
    String address;
    int rssi;
    String manufacturer;
    String deviceType;
};

static std::vector<BLEDeviceInfo> devices;
static BLEScan *pBLEScan;
static int selectedIndex = 0;

// ===== CALLBACK =====
class MyAdvertisedDeviceCallbacks : public BLEAdvertisedDeviceCallbacks {
    void onResult(BLEAdvertisedDevice advertisedDevice) override {
        BLEDeviceInfo dev;

        String tempName = advertisedDevice.getName();

        if (tempName.length() == 0 && advertisedDevice.haveServiceData()) {
            tempName = advertisedDevice.getServiceData().c_str();
        }

        if (tempName.length() == 0) {
            String addr = advertisedDevice.getAddress().toString().c_str();
            tempName = "BLE_" + addr.substring(addr.length() - 5);
        }

        dev.name = tempName;
        dev.address = advertisedDevice.getAddress().toString().c_str();
        dev.rssi = advertisedDevice.getRSSI();

        if (advertisedDevice.haveManufacturerData()) {
            String mData = advertisedDevice.getManufacturerData().c_str();

            if (mData.length() >= 2) {
                char buffer[10];
                sprintf(buffer, "0x%02X%02X",
                        (uint8_t)mData[1],
                        (uint8_t)mData[0]);
                dev.manufacturer = String(buffer);
            } else {
                dev.manufacturer = "unknown";
            }
        } else {
            dev.manufacturer = "unknown";
        }

        if (advertisedDevice.haveServiceUUID()) {
            dev.deviceType =
                advertisedDevice.getServiceUUID().toString().c_str();
        } else {
            dev.deviceType = "unknown";
        }

        devices.push_back(dev);
    }
};

// ===== DRAW MENU =====
void ble_drawMenu()
{
    u8g2.clearBuffer();

    if (devices.empty())
    {
        u8g2.setFont(u8g2_font_6x12_tr);
        u8g2.drawStr(0, 30, "No devices");
        u8g2.drawStr(0, 45, "Press BACK");
    }
    else
    {
        u8g2.setFont(u8g2_font_5x8_tr);

        char counter[20];
        sprintf(counter, "%d/%d", selectedIndex + 1, (int)devices.size());
        u8g2.drawStr(0, 8, counter);

        u8g2.setFont(u8g2_font_6x10_tr);

        for (int i = 0; i < 3; i++)
        {
            int idx = selectedIndex + i;
            if (idx >= devices.size()) break;

            int y = 22 + i * 14;

            if (i == 0)
            {
                u8g2.drawBox(0, y - 10, 128, 12);
                u8g2.setDrawColor(0);
            }

            String text = devices[idx].name;
            if (text.length() > 12)
                text = text.substring(0, 12) + "..";

            text += " (" + String(devices[idx].rssi) + ")";

            u8g2.drawStr(2, y, text.c_str());

            if (i == 0)
                u8g2.setDrawColor(1);
        }
    }

    u8g2.sendBuffer();
}

// ===== DEVICE DETAILS =====
void ble_drawDetails(const BLEDeviceInfo &dev)
{
    u8g2.clearBuffer();
    u8g2.setFont(u8g2_font_5x8_tr);

    u8g2.drawStr(0, 10, dev.name.c_str());
    u8g2.drawStr(0, 20, dev.address.c_str());

    char rssiStr[20];
    sprintf(rssiStr, "RSSI: %d", dev.rssi);
    u8g2.drawStr(0, 30, rssiStr);

    u8g2.drawStr(0, 40, dev.manufacturer.c_str());

    u8g2.sendBuffer();
}

// ===== SCAN =====
void ble_scan()
{
    devices.clear();

    u8g2.clearBuffer();
    u8g2.drawStr(10, 30, "Scanning...");
    u8g2.sendBuffer();

    BLEDevice::init("");

    pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks(), false);
    pBLEScan->setActiveScan(true);
    pBLEScan->setInterval(100);
    pBLEScan->setWindow(99);

    pBLEScan->start(5, false);

    // remove duplicates
    std::vector<BLEDeviceInfo> unique;
    for (auto &d : devices)
    {
        bool exists = false;
        for (auto &u : unique)
        {
            if (u.address == d.address)
            {
                exists = true;
                break;
            }
        }
        if (!exists) unique.push_back(d);
    }

    devices = unique;
}

// ===== MAIN LOOP =====
void ble_loop()
{
    static uint32_t lastPress = 0;

    if (millis() - lastPress < 200)
        return;

    if (btnDown() && selectedIndex < (int)devices.size() - 1)
    {
        selectedIndex++;
        ble_drawMenu();
        lastPress = millis();
    }
    else if (btnUp() && selectedIndex > 0)
    {
        selectedIndex--;
        ble_drawMenu();
        lastPress = millis();
    }
    else if (btnSelect() && !devices.empty())
    {
        ble_drawDetails(devices[selectedIndex]);
        delay(3000);
        ble_drawMenu();
        lastPress = millis();
    }
    else if (btnBack())
    {
        lastPress = millis();
        return;
    }
}
