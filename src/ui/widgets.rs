//! Reusable screens drawn with the [`Display`] primitives.
//!
//! Coordinates mirror the Arduino reference so the ported UI looks the same.

use crate::drivers::display::Display;

/// Boot splash (parity: `src-arduino/main.cpp` `showSplash`).
pub fn splash(d: &mut Display) {
    d.clear();
    d.title(10, 40, "Orion-RF");
    d.text(25, 58, "Initializing...");
    d.flush();
}

/// Placeholder screen for features not yet ported. Replaced by the real app in
/// its phase; until then every unimplemented menu entry lands here.
pub fn stub(d: &mut Display, name: &str) {
    d.clear();
    d.text(0, 14, name);
    d.text(0, 34, "Not implemented");
    d.text(0, 54, "BACK to return");
    d.flush();
}
