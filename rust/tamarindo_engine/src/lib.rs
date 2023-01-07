// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod render;

use render::Renderer;
use winit::{
    event::*,
    event_loop::{ControlFlow, EventLoop},
    window::{Window, WindowBuilder},
};

use log::error;

pub struct Application {
    event_loop: EventLoop<()>,
    window: Window,
    renderer: Renderer,
}

pub enum NewApplicationError {
    Failed,
}

impl Application {
    pub fn new() -> Result<Self, NewApplicationError> {
        let event_loop = EventLoop::new();
        let window_builder = WindowBuilder::new().with_title("Tamarindo Engine");
        // todo: handle this error
        let window = window_builder.build(&event_loop).unwrap();

        let renderer = pollster::block_on(Renderer::new(&window));

        Ok(Self {
            event_loop,
            window,
            renderer,
        })
    }

    pub fn start(mut self) {
        self.event_loop
            .run(move |event, _, control_flow| match event {
                Event::WindowEvent {
                    ref event,
                    window_id,
                } if window_id == self.window.id() => match event {
                    // todo: Move input events outside and match return value
                    WindowEvent::CloseRequested
                    | WindowEvent::KeyboardInput {
                        input:
                            KeyboardInput {
                                state: ElementState::Pressed,
                                virtual_keycode: Some(VirtualKeyCode::Escape),
                                ..
                            },
                        ..
                    } => *control_flow = ControlFlow::Exit,
                    WindowEvent::Resized(physical_size) => {
                        self.renderer.resize(*physical_size);
                    }
                    WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                        self.renderer.resize(**new_inner_size);
                    }
                    _ => {}
                },
                Event::RedrawRequested(window_id) if window_id == self.window.id() => {
                    // todo: call update?
                    match self.renderer.render() {
                        Ok(_) => {}
                        // Reconfigure the surface if lost (todo: refactor this to not expose size)
                        Err(wgpu::SurfaceError::Lost) => self.renderer.resize(self.renderer.size),
                        // The system is out of memory, we should probably quit
                        Err(wgpu::SurfaceError::OutOfMemory) => *control_flow = ControlFlow::Exit,
                        // All other errors (Outdated, Timeout) should be resolved by the next frame
                        Err(e) => error!("{:?}", e),
                    }
                }
                Event::MainEventsCleared => {
                    // RedrawRequested will only trigger once, unless we manually
                    // request it.
                    self.window.request_redraw();
                }
                _ => {}
            });
    }
}
