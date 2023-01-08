// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod buffer;
mod render_pass;
mod texture;

use buffer::PosWithUvBuffer;
use log::debug;
use render_pass::RenderPass;
use texture::{Texture, TextureBindGroup};
use winit::window::Window;

pub struct Renderer {
    surface: wgpu::Surface,
    device: wgpu::Device,
    queue: wgpu::Queue,
    config: wgpu::SurfaceConfiguration,
    pub size: winit::dpi::PhysicalSize<u32>,
    render_pass: RenderPass,
}

impl Renderer {
    pub async fn new(window: &Window) -> Self {
        let size = window.inner_size();
        debug!("{:?}", size);

        let instance = wgpu::Instance::new(wgpu::Backends::all());
        let surface = unsafe { instance.create_surface(window) };
        let adapter = instance
            .request_adapter(&wgpu::RequestAdapterOptions {
                power_preference: wgpu::PowerPreference::default(),
                compatible_surface: Some(&surface),
                force_fallback_adapter: false,
            })
            .await
            .unwrap();

        let (device, queue) = adapter
            .request_device(
                &wgpu::DeviceDescriptor {
                    features: wgpu::Features::empty(),
                    limits: wgpu::Limits::default(),
                    label: None,
                },
                None, // Trace path
            )
            .await
            .unwrap();

        let config = wgpu::SurfaceConfiguration {
            usage: wgpu::TextureUsages::RENDER_ATTACHMENT,
            format: surface.get_supported_formats(&adapter)[0],
            width: size.width,
            height: size.height,
            present_mode: wgpu::PresentMode::Fifo,
            alpha_mode: wgpu::CompositeAlphaMode::Auto,
        };
        surface.configure(&device, &config);
        debug!("{:?}", adapter);

        let shader = device.create_shader_module(wgpu::ShaderModuleDescriptor {
            label: Some("My Shader"),
            source: wgpu::ShaderSource::Wgsl(include_str!("../../res/shaders/shader.wgsl").into()),
        });

        let diffuse_bytes = include_bytes!("../../res/img/3crates/crate1/crate1_diffuse.png");
        // todo: handle this error
        let diffuse_texture =
            Texture::new_from_bytes(&device, &queue, diffuse_bytes, "happy-tree.png").unwrap();
        let crate_diffuse_bind_group =
            TextureBindGroup::new_diffuse_bind_group(&device, diffuse_texture, "crate");

        let object = PosWithUvBuffer::new_square(&device);

        let render_pass = RenderPass::new(
            &device,
            crate_diffuse_bind_group,
            shader,
            object,
            config.format,
            "crate",
        );

        Self {
            surface,
            device,
            queue,
            config,
            size,
            render_pass,
        }
    }

    pub fn resize(&mut self, new_size: winit::dpi::PhysicalSize<u32>) {
        if new_size.width > 0 && new_size.height > 0 {
            self.size = new_size;
            self.config.width = new_size.width;
            self.config.height = new_size.height;
            self.surface.configure(&self.device, &self.config);
        }
    }

    pub fn render(&mut self) -> Result<(), wgpu::SurfaceError> {
        // todo: catch this error and return custom error enum
        let output = self.surface.get_current_texture()?;

        let mut encoder = self
            .device
            .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                label: Some("render_encoder"),
            });

        self.render_pass.record_render_pass(
            &mut encoder,
            output
                .texture
                .create_view(&wgpu::TextureViewDescriptor::default()),
        );

        // submit will accept anything that implements IntoIter
        self.queue.submit(std::iter::once(encoder.finish()));
        output.present();

        Ok(())
    }
}
