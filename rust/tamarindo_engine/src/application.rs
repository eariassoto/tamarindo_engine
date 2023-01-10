// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use log::{debug, error};
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{
    application_config::ApplicationConfig,
    render::{
        buffer::PosWithUvBuffer,
        render_pass::RenderPass,
        shader::Shader,
        texture::{Texture, TextureBindGroup},
        Renderer,
    },
    Error,
};

pub struct Application {
    window: Window,
    renderer: Renderer,
}

impl Application {
    pub fn new(app_config: ApplicationConfig, window: Window) -> Result<Self, Error> {
        debug!("{:?}", app_config.vertex_data);
        let mut renderer = pollster::block_on(Renderer::new(&window));
        let shader = Shader::new(
            "crate_box",
            include_str!("../res/shaders/shader.wgsl"),
            &renderer,
        );

        // todo: handle this error
        let diffuse_texture = Texture::new_from_bytes(
            &renderer.device,
            &renderer.queue,
            include_bytes!("../res/img/3crates/crate1/crate1_diffuse.png"),
            "crate1_diffuse.png",
        )
        .unwrap();
        let crate_diffuse_bind_group =
            TextureBindGroup::new_diffuse_bind_group(&renderer.device, diffuse_texture, "crate");

        let object = PosWithUvBuffer::new_square(&renderer.device);

        let render_pass = RenderPass::new(
            &renderer.device,
            crate_diffuse_bind_group,
            shader,
            object,
            renderer.config.format,
            "crate",
        );
        renderer.render_passes.push(render_pass);

        Ok(Self { window, renderer })
    }

    pub fn process_event(&mut self, event: &Event<'_, ()>, control_flow: &mut ControlFlow) {
        match event {
            Event::WindowEvent {
                ref event,
                window_id,
            } if *window_id == self.window.id() => match event {
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
            Event::RedrawRequested(window_id) if *window_id == self.window.id() => {
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
        }
    }
}
