
mod board;
mod drivers;

use drivers::display::Display;

fn main() {
    // It is necessary to call this function once. Otherwise, some patches to the runtime
    // implemented by esp-idf-sys might not link properly. See https://github.com/esp-rs/esp-idf-template/issues/71
    esp_idf_svc::sys::link_patches();

    // Bind the log crate to the ESP Logging facilities
    esp_idf_svc::log::EspLogger::initialize_default();

    log::info!("ORION-RF Booted!!");
    let mut display = Display::new();

    display.init();
    display.draw_text("ORION-RF");

}
