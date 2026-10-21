//! Scrolling menu + submenu navigation and feature dispatch.
//!
//! Parity reference: `src-arduino/ui/menu.cpp`. Four visible rows, a `>`
//! cursor, `^`/`v` scroll indicators, wrap-around up/down, and the same item
//! order. Selecting BadUSB or NRF Tools enters a submenu; BACK from a submenu
//! returns to the main menu. Restart shows a YES/NO confirm dialog.
//!
//! Phase 1 dispatches every feature except the submenu entries and Restart to
//! [`StubApp`]; later phases swap in the real apps.

use esp_idf_hal::delay::FreeRtos;

use crate::apps::{App, AppCtx, StubApp};
use crate::board::board::Board;
use crate::drivers::buttons::{debounce, Buttons};
use crate::drivers::display::Display;

const VISIBLE_ROWS: usize = 4;

const MAIN: &[&str] = &[
    "BadUSB",
    "RF Capture",
    "NRF Tools",
    "BLE Scan",
    "Wifi Scan",
    "Wifi Analyzer",
    "System Info",
    "Device Check",
    "Restart",
    "Ble Mouse",
    "NFC Tools",
];

const NRF_TOOLS: &[&str] = &[
    "BLE Jammer",
    "Bluetooth Jammer",
    "Wifi Jammer",
    "USB Wireless",
    "Video TX",
    "Zigbee",
    "RC",
];

const BADUSB: &[&str] = &[
    "ORION Demo",
    "RickRoll",
    "Matrix Rain",
    "Fake Terminal",
    "Wifi Password Extractor",
    "Fake Update",
    "Fake BSOD",
    "Glitch Screen",
    "ASCII Spam",
    "Hacker Typer",
    "PWN Reverse shell",
    "Phishing attack",
    "Desktop Ghost",
    "System Stresser",
];

#[derive(Clone, Copy, PartialEq)]
enum Screen {
    Main,
    Nrf,
    Badusb,
}

struct Nav {
    screen: Screen,
    index: usize,
    offset: usize,
}

impl Nav {
    fn items(&self) -> &'static [&'static str] {
        match self.screen {
            Screen::Main => MAIN,
            Screen::Nrf => NRF_TOOLS,
            Screen::Badusb => BADUSB,
        }
    }

    fn enter(&mut self, screen: Screen) {
        self.screen = screen;
        self.index = 0;
        self.offset = 0;
    }
}

/// Run the menu forever. This is the main loop of the device.
pub fn run(display: &mut Display, buttons: &mut Buttons, board: &mut Board) -> ! {
    let mut nav = Nav {
        screen: Screen::Main,
        index: 0,
        offset: 0,
    };
    draw(display, &mut nav);

    loop {
        if buttons.up() {
            let len = nav.items().len();
            nav.index = if nav.index == 0 { len - 1 } else { nav.index - 1 };
            draw(display, &mut nav);
            debounce();
        } else if buttons.down() {
            nav.index += 1;
            if nav.index >= nav.items().len() {
                nav.index = 0;
            }
            draw(display, &mut nav);
            debounce();
        } else if buttons.select() {
            debounce();
            launch(display, buttons, board, &mut nav);
            draw(display, &mut nav);
        } else if buttons.back() {
            if nav.screen != Screen::Main {
                nav.enter(Screen::Main);
                draw(display, &mut nav);
            }
            debounce();
        } else {
            FreeRtos::delay_ms(20);
        }
    }
}

/// Draw the current menu with scroll handling (parity: `drawMenu`).
fn draw(display: &mut Display, nav: &mut Nav) {
    let items = nav.items();

    if nav.index < nav.offset {
        nav.offset = nav.index;
    }
    if nav.index >= nav.offset + VISIBLE_ROWS {
        nav.offset = nav.index - VISIBLE_ROWS + 1;
    }

    display.clear();

    for i in 0..VISIBLE_ROWS {
        let item = nav.offset + i;
        if item >= items.len() {
            break;
        }
        let y = 14 + i as i32 * 14;
        if item == nav.index {
            display.text(0, y, ">");
        }
        display.text(10, y, items[item]);
    }

    if nav.offset > 0 {
        display.text(118, 10, "^");
    }
    if nav.offset + VISIBLE_ROWS < items.len() {
        display.text(118, 62, "v");
    }

    display.flush();
}

/// Act on SELECT in the current screen (parity: `launchFeature`).
fn launch(display: &mut Display, buttons: &mut Buttons, board: &mut Board, nav: &mut Nav) {
    match nav.screen {
        Screen::Main => match nav.index {
            0 => nav.enter(Screen::Badusb),
            2 => nav.enter(Screen::Nrf),
            8 => confirm_restart(display, buttons),
            other => run_stub(display, buttons, board, MAIN[other]),
        },
        Screen::Nrf => run_stub(display, buttons, board, NRF_TOOLS[nav.index]),
        Screen::Badusb => run_stub(display, buttons, board, BADUSB[nav.index]),
    }
}

fn run_stub(display: &mut Display, buttons: &mut Buttons, board: &mut Board, name: &'static str) {
    let mut app = StubApp { name };
    let mut ctx = AppCtx {
        display,
        buttons,
        board,
    };
    let _ = app.run(&mut ctx);
}

/// YES/NO restart confirmation (parity: `menu.cpp` case 8).
fn confirm_restart(d: &mut Display, b: &mut Buttons) {
    // Wait for the SELECT that opened this dialog to be released.
    FreeRtos::delay_ms(200);
    while b.select() {
        FreeRtos::delay_ms(10);
    }

    let mut confirm = false;

    loop {
        d.clear();
        d.text(18, 18, "Restart Device?");

        if confirm {
            d.fill_rect(10, 35, 45, 15);
            d.text_rev(20, 47, "YES");
            d.text(75, 47, "NO");
        } else {
            d.text(20, 47, "YES");
            d.fill_rect(65, 35, 45, 15);
            d.text_rev(78, 47, "NO");
        }

        d.flush();

        if b.left() || b.up() {
            confirm = true;
            debounce();
        }
        if b.right() || b.down() {
            confirm = false;
            debounce();
        }

        if b.select() {
            debounce();
            if confirm {
                d.clear();
                d.text(28, 30, "Restarting...");
                d.flush();
                FreeRtos::delay_ms(1000);
                esp_idf_hal::reset::restart();
            } else {
                break;
            }
        }

        if b.back() {
            debounce();
            break;
        }

        FreeRtos::delay_ms(10);
    }
}
