// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use winit::{
    event_loop::EventLoop,
    window::{Window, WindowBuilder},
};

use crate::EngineError;

pub struct WindowState {
    pub event_loop: EventLoop<()>,
    pub window: Window,
}

impl WindowState {
    pub fn new(
        window_name: String,
        window_width: u32,
        window_height: u32,
    ) -> Result<Self, EngineError> {
        let event_loop = EventLoop::new();
        let window_builder = WindowBuilder::new()
            .with_title(window_name)
            .with_inner_size(winit::dpi::LogicalSize::new(window_width, window_height));

        let window = match window_builder.build(&event_loop) {
            Ok(w) => w,
            Err(e) => return Err(EngineError::CreateWinitWindowError(e)),
        };

        Ok(Self {
            event_loop: event_loop,
            window: window,
        })
    }
}
