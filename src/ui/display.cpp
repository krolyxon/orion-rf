#include "display.h"
#include "config.h"
#include <Wire.h>

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, U8X8_PIN_NONE);

void displayInit() {
  Wire.begin(OLED_SDA_PIN, OLED_SCL_PIN);
  u8g2.begin();
  u8g2.setFont(u8g2_font_6x12_tr);
}
