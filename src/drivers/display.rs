use crate::board::pins::*;

pub struct Display {
}

impl Display {
    pub fn new() -> Self {
        Self {}
    }

    pub fn init(&mut self) {
        log::info!("display Initialized!!");
    }

    pub fn draw_text(&mut self, text: &str) {
        log::info!("{}", text);
    }
}
