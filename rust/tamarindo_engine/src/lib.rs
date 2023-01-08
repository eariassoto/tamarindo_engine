// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod render;

use log::{debug, error};
use render::Renderer;
use serde::{Deserialize, Serialize};
use winit::{
    event::*,
    event_loop::{ControlFlow, EventLoop},
    window::{Window, WindowBuilder},
};

#[derive(Debug, PartialEq, Serialize, Deserialize)]
struct ApplicationConfig {
    app_name: String,
    window_width: u32,
    window_height: u32,
    // todo: fix this
    vertex_data: Vec<f32>,
    index_data: Vec<u16>,
}

pub struct Application {
    event_loop: EventLoop<()>,
    window: Window,
    renderer: Renderer,
}

pub enum ApplicationNewError {
    InvalidConfig(serde_yaml::Error),
    CannotCreateWindow,
}

impl Application {
    pub fn new_from_config_str(config: &str) -> Result<Self, ApplicationNewError> {
        let app_config = match serde_yaml::from_str::<ApplicationConfig>(&config) {
            Ok(config) => config,
            Err(e) => return Err(ApplicationNewError::InvalidConfig(e)),
        };
        debug!("{:?}", app_config.vertex_data);

        let event_loop = EventLoop::new();
        let window_builder = WindowBuilder::new()
            .with_title(app_config.app_name)
            .with_inner_size(winit::dpi::LogicalSize::new(
                app_config.window_width,
                app_config.window_height,
            ));
        // todo: handle this error
        let window = match window_builder.build(&event_loop) {
            Ok(w) => w,
            Err(_) => return Err(ApplicationNewError::CannotCreateWindow),
        };

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
