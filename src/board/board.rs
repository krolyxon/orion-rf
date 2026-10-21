//! The one owned `Board`, built in `main` from `Peripherals::take()`.
//!
//! It owns the shared buses and the peripheral/pin handles that later phases
//! consume (radios, CC1101, WiFi/BLE modem). Display and Buttons are built as
//! siblings in `main` and reach apps via `AppCtx`, not through `Board`.
//!
//! The shared I2C lives behind a `'static` `RefCell` so SSD1306 (always-on) and
//! PN532 can both hold `RefCellDevice`s over it without re-initialising the bus.

use core::cell::RefCell;

use esp_idf_hal::gpio::AnyIOPin;
use esp_idf_hal::i2c::I2cDriver;
use esp_idf_hal::modem::Modem;
use esp_idf_hal::spi::{SPI2, SPI3};

/// Shared handle to the I2C0 bus (OLED + PN532). Leaked to `'static` in `main`
/// because the display is always on for the lifetime of the program.
pub type SharedI2c = &'static RefCell<I2cDriver<'static>>;

/// NRF24 bus + control pins (SPI2). Consumed in Phase 4.
pub struct NrfPins {
    pub sck: AnyIOPin<'static>,
    pub miso: AnyIOPin<'static>,
    pub mosi: AnyIOPin<'static>,
    pub ce1: AnyIOPin<'static>,
    pub csn1: AnyIOPin<'static>,
    pub ce2: AnyIOPin<'static>,
    pub csn2: AnyIOPin<'static>,
}

/// CC1101 bus + control pins (SPI3). Consumed in Phase 4.
pub struct Cc1101Pins {
    pub sck: AnyIOPin<'static>,
    pub miso: AnyIOPin<'static>,
    pub mosi: AnyIOPin<'static>,
    pub cs: AnyIOPin<'static>,
    pub gdo0: AnyIOPin<'static>,
    pub gdo2: AnyIOPin<'static>,
}

/// Owns the peripherals and bus handles not already consumed by the display and
/// buttons. Passed `&mut` into every app via `AppCtx`.
pub struct Board {
    /// Shared I2C0 bus (OLED is live; PN532 joins in Phase 4).
    pub i2c: SharedI2c,
    /// WiFi/BLE radio — acquired/released by one app at a time (Phase 2/3).
    pub modem: Modem<'static>,
    /// SPI2 host for the two NRF24 radios (Phase 4).
    pub spi2: SPI2<'static>,
    /// SPI3 host for the CC1101 (Phase 4).
    pub spi3: SPI3<'static>,
    pub nrf: NrfPins,
    pub cc1101: Cc1101Pins,
}
