//! SSD1306 128x64 OLED over the shared I2C0 bus.
//!
//! Parity reference: `src-arduino/ui/display.cpp` (u8g2 `..._F_HW_I2C`, font
//! `u8g2_font_6x12_tr`). We approximate u8g2 with `embedded-graphics`
//! `MonoTextStyle` + `FONT_6X10`, keeping the Arduino draw coordinates so menu
//! layout matches 1:1. Drawing uses u8g2-style baseline coordinates
//! (`Baseline::Alphabetic`): the given `y` is the text baseline, as in
//! `u8g2.drawStr(x, y, ...)`.

use embedded_graphics::{
    mono_font::{
        ascii::{FONT_6X10, FONT_9X18_BOLD},
        MonoFont, MonoTextStyle,
    },
    pixelcolor::BinaryColor,
    prelude::*,
    primitives::{PrimitiveStyle, Rectangle},
    text::{Baseline, Text},
};
use embedded_hal_bus::i2c::RefCellDevice;
use esp_idf_hal::i2c::I2cDriver;
use ssd1306::{
    mode::{BufferedGraphicsMode, DisplayConfig},
    prelude::*,
    I2CDisplayInterface, Ssd1306,
};

use crate::board::board::SharedI2c;

/// The I2C device the OLED talks over: a `RefCellDevice` sharing the `'static`
/// bus so PN532 can later take a second device on the same `RefCell`.
type OledI2c = RefCellDevice<'static, I2cDriver<'static>>;

type Oled =
    Ssd1306<I2CInterface<OledI2c>, DisplaySize128x64, BufferedGraphicsMode<DisplaySize128x64>>;

pub struct Display {
    raw: Oled,
}

impl Display {
    /// Build + initialise the SSD1306 over the shared bus and clear the screen.
    pub fn new(bus: SharedI2c) -> Self {
        let dev = RefCellDevice::new(bus);
        let iface = I2CDisplayInterface::new(dev);
        let mut raw = Ssd1306::new(iface, DisplaySize128x64, DisplayRotation::Rotate0)
            .into_buffered_graphics_mode();
        raw.init().expect("ssd1306 init");

        let mut display = Self { raw };
        display.clear();
        display.flush();
        display
    }

    /// Clear the off-screen buffer (does not touch the panel until `flush`).
    pub fn clear(&mut self) {
        self.raw.clear_buffer();
    }

    /// Push the off-screen buffer to the panel (u8g2 `sendBuffer`).
    pub fn flush(&mut self) {
        let _ = self.raw.flush();
    }

    /// Normal text: foreground pixels on, baseline at `y`.
    pub fn text(&mut self, x: i32, y: i32, s: &str) {
        self.draw_text(x, y, s, &FONT_6X10, BinaryColor::On);
    }

    /// Reversed text (off pixels) for drawing over a filled box
    /// (u8g2 `setDrawColor(0)`).
    pub fn text_rev(&mut self, x: i32, y: i32, s: &str) {
        self.draw_text(x, y, s, &FONT_6X10, BinaryColor::Off);
    }

    /// Large title text (approximates u8g2 `logisoso20`) for the splash screen.
    pub fn title(&mut self, x: i32, y: i32, s: &str) {
        self.draw_text(x, y, s, &FONT_9X18_BOLD, BinaryColor::On);
    }

    /// Filled rectangle (u8g2 `drawBox`).
    pub fn fill_rect(&mut self, x: i32, y: i32, w: u32, h: u32) {
        let _ = Rectangle::new(Point::new(x, y), Size::new(w, h))
            .into_styled(PrimitiveStyle::with_fill(BinaryColor::On))
            .draw(&mut self.raw);
    }

    fn draw_text(&mut self, x: i32, y: i32, s: &str, font: &MonoFont<'static>, color: BinaryColor) {
        let style = MonoTextStyle::new(font, color);
        let _ = Text::with_baseline(s, Point::new(x, y), style, Baseline::Alphabetic)
            .draw(&mut self.raw);
    }
}
