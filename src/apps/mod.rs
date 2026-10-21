//! The `App` abstraction: each menu feature owns its own render/poll loop and
//! returns when the user presses BACK.
//!
//! This replaces the Arduino per-feature blocking `while(1)` loops. An app
//! borrows the shared resources through [`AppCtx`] for the duration of `run`
//! and releases them on return, so the menu regains control afterwards.

use esp_idf_hal::delay::FreeRtos;

use crate::board::board::Board;
use crate::drivers::buttons::{debounce, Buttons};
use crate::drivers::display::Display;
use crate::ui::widgets;

/// How an app finished. Only BACK exists for now; future apps may add variants
/// (e.g. requesting a restart).
pub enum AppResult {
    Back,
}

/// Everything an app may touch, borrowed for the length of `run`.
pub struct AppCtx<'a> {
    pub display: &'a mut Display,
    pub buttons: &'a mut Buttons,
    pub board: &'a mut Board,
}

pub trait App {
    fn run(&mut self, ctx: &mut AppCtx) -> AppResult;
}

/// Placeholder app for features awaiting their phase. Shows a name screen and
/// waits for BACK.
pub struct StubApp {
    pub name: &'static str,
}

impl App for StubApp {
    fn run(&mut self, ctx: &mut AppCtx) -> AppResult {
        widgets::stub(ctx.display, self.name);
        loop {
            if ctx.buttons.back() {
                debounce();
                return AppResult::Back;
            }
            FreeRtos::delay_ms(20);
        }
    }
}
