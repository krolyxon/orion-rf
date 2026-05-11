#include <Arduino.h>
#include "../ui/display.h"
#include "buttons.h"
#include <esp_chip_info.h>
#include <esp_heap_caps.h>

void runSystemInfoFeature()
{
    esp_chip_info_t chip_info;

    esp_chip_info(&chip_info);

    while (true)
    {
        //u8g2.clearBuffer();

        //char buf[32];

        //sprintf(buf, "Cores: %d", chip_info.cores);
        //u8g2.drawStr(0, 14, buf);

        //sprintf(buf, "Heap: %d",
        //        heap_caps_get_free_size(MALLOC_CAP_DEFAULT));
        //u8g2.drawStr(0, 28, buf);


        //u8g2.drawStr(0, 60, "BACK to exit");

          // Get RAM info
          size_t freeHeap = heap_caps_get_free_size(MALLOC_CAP_DEFAULT);
          size_t totalHeap = heap_caps_get_total_size(MALLOC_CAP_DEFAULT);
          int ramUsage = 100 - ((freeHeap * 100) / totalHeap);

          // Get Flash info
         //uint32_t flashSize = spi_flash_get_chip_size();
          uint32_t flashSize = ESP.getFlashChipSize();
          uint32_t flashUsed = ESP.getSketchSize();
          int flashUsage = (flashUsed * 100) / flashSize;

          // Temperature (approx)
          uint8_t temperature = temperatureRead();

          // Chip info
          esp_chip_info_t chip_info;
          esp_chip_info(&chip_info);

          u8g2.clearBuffer();
          u8g2.setFont(u8g2_font_6x12_tr);

         char buf[32];

        // Box 1 - RAM
        u8g2.drawFrame(0, 0, 128, 12);
        sprintf(buf, "RAM: %d%% used", ramUsage);
        u8g2.drawStr(4, 9, buf);

        // Box 2 - Flash
        u8g2.drawFrame(0, 12, 128, 12);
        sprintf(buf, "Flash: %d%% used", flashUsage);
        u8g2.drawStr(4, 21, buf);

        // Box 3 - Temp (FULL WIDTH now)
        u8g2.drawFrame(0, 24, 128, 12);
        sprintf(buf, "Temp: %d C", temperature);
        u8g2.drawStr(4, 33, buf);

        // Box 4 - Chip info (FULL WIDTH)
        u8g2.drawFrame(0, 36, 128, 12);
        sprintf(buf, "Cores: %d Rev: %d", chip_info.cores, chip_info.revision);
        u8g2.drawStr(4, 45, buf);

        // Box 5 - PSRAM (KB)
        u8g2.drawFrame(0, 48, 128, 12);
        sprintf(buf, "PSRAM: %lu KB", heap_caps_get_free_size(MALLOC_CAP_SPIRAM) / 1024);
        u8g2.drawStr(4, 57, buf);

        u8g2.sendBuffer();

        if (btnBack())
        {
            delay(200);
            return;
        }

        delay(100);
    }
}
