//! Six momentary buttons, active-low with internal pull-ups.
//!
//! Parity reference: `src-arduino/utils/buttons.cpp`. Arduino uses
//! `pinMode(INPUT_PULLUP)` and reports pressed as `!digitalRead(pin)`, i.e. a
//! pressed button reads LOW. We mirror that: `get_level() == Level::Low`.
//!
//! Debounce (the Arduino `delay(150)` after each handled press) lives at the
//! call site via [`debounce`], matching the original per-action delays.

use esp_idf_hal::delay::FreeRtos;
use esp_idf_hal::gpio::{AnyIOPin, Input, Level, PinDriver, Pull};

/// The ~150ms debounce the Arduino firmware applies after each handled press.
pub const DEBOUNCE_MS: u32 = 150;

/// Block for the standard debounce interval after acting on a press.
pub fn debounce() {
    FreeRtos::delay_ms(DEBOUNCE_MS);
}

type Btn = PinDriver<'static, Input>;

pub struct Buttons {
    up: Btn,
    down: Btn,
    select: Btn,
    back: Btn,
    right: Btn,
    left: Btn,
}

impl Buttons {
    /// Configure the six pins as pulled-up inputs. Order matches the physical
    /// labels, not GPIO numbers (see `board::pins`).
    pub fn new(
        up: AnyIOPin<'static>,
        down: AnyIOPin<'static>,
        select: AnyIOPin<'static>,
        back: AnyIOPin<'static>,
        right: AnyIOPin<'static>,
        left: AnyIOPin<'static>,
    ) -> Self {
        Self {
            up: Self::input(up),
            down: Self::input(down),
            select: Self::input(select),
            back: Self::input(back),
            right: Self::input(right),
            left: Self::input(left),
        }
    }

    fn input(pin: AnyIOPin<'static>) -> Btn {
        PinDriver::input(pin, Pull::Up).expect("button input")
    }

    pub fn up(&self) -> bool {
        self.up.get_level() == Level::Low
    }
    pub fn down(&self) -> bool {
        self.down.get_level() == Level::Low
    }
    pub fn select(&self) -> bool {
        self.select.get_level() == Level::Low
    }
    pub fn back(&self) -> bool {
        self.back.get_level() == Level::Low
    }
    pub fn right(&self) -> bool {
        self.right.get_level() == Level::Low
    }
    pub fn left(&self) -> bool {
        self.left.get_level() == Level::Low
    }
}
