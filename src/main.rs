// Scaffold phase of the Arduino -> Rust rewrite: later phases consume the
// reserved Board peripherals/pins, so allow the not-yet-read fields for now.
#![allow(dead_code)]

mod apps;
mod board;
mod drivers;
mod ui;

use core::cell::RefCell;

use esp_idf_hal::i2c::{config::Config as I2cConfig, I2cDriver};
use esp_idf_hal::peripherals::Peripherals;
use esp_idf_hal::units::FromValueType;

use board::board::{Board, Cc1101Pins, NrfPins, SharedI2c};
use drivers::buttons::Buttons;
use drivers::display::Display;

fn main() {
    // Required once so esp-idf-sys runtime patches link. See
    // https://github.com/esp-rs/esp-idf-template/issues/71
    esp_idf_svc::sys::link_patches();
    esp_idf_svc::log::EspLogger::initialize_default();

    log::info!("ORION-RF booting");

    let peripherals = Peripherals::take().expect("take peripherals");
    let pins = peripherals.pins;

    // Shared I2C0 bus (OLED + PN532). Leaked to 'static: the display is on for
    // the whole program, so this bus never needs to be dropped or re-created.
    let i2c = I2cDriver::new(
        peripherals.i2c0,
        pins.gpio8, // SDA
        pins.gpio9, // SCL
        &I2cConfig::new().baudrate(400u32.kHz().into()),
    )
    .expect("i2c0 init");
    let i2c_bus: SharedI2c = Box::leak(Box::new(RefCell::new(i2c)));

    let mut display = Display::new(i2c_bus);

    // Buttons by physical label (active-low, internal pull-ups).
    let mut buttons = Buttons::new(
        pins.gpio4.degrade_input_output(), // UP
        pins.gpio5.degrade_input_output(), // DOWN
        pins.gpio6.degrade_input_output(), // SELECT
        pins.gpio7.degrade_input_output(), // BACK
        pins.gpio1.degrade_input_output(), // RIGHT
        pins.gpio2.degrade_input_output(), // LEFT
    );

    // Everything the radio/NFC phases will consume, owned up front.
    let mut board = Board {
        i2c: i2c_bus,
        modem: peripherals.modem,
        spi2: peripherals.spi2,
        spi3: peripherals.spi3,
        nrf: NrfPins {
            sck: pins.gpio18.degrade_input_output(),
            miso: pins.gpio16.degrade_input_output(),
            mosi: pins.gpio17.degrade_input_output(),
            ce1: pins.gpio10.degrade_input_output(),
            csn1: pins.gpio11.degrade_input_output(),
            ce2: pins.gpio12.degrade_input_output(),
            csn2: pins.gpio13.degrade_input_output(),
        },
        cc1101: Cc1101Pins {
            sck: pins.gpio15.degrade_input_output(),
            miso: pins.gpio41.degrade_input_output(),
            mosi: pins.gpio35.degrade_input_output(),
            cs: pins.gpio40.degrade_input_output(),
            gdo0: pins.gpio39.degrade_input_output(),
            gdo2: pins.gpio42.degrade_input_output(),
        },
    };

    ui::widgets::splash(&mut display);
    esp_idf_hal::delay::FreeRtos::delay_ms(1500);

    log::info!("ORION-RF ready");
    ui::menu::run(&mut display, &mut buttons, &mut board);
}
