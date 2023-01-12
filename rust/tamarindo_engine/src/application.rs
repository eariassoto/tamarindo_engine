// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::time::Instant;

use cgmath::Vector3;
use log::debug;
use wgpu::util::DeviceExt;
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{
    application_config::ApplicationConfig,
    camera::OrthographicCamera,
    instance::Instance,
    render::{
        buffer::PosWithUvBuffer, render_pass::RenderPass, shader::Shader, texture::Texture,
        Renderer,
    },
    Error,
};

pub struct Application {
    window: Window,
    renderer: Renderer,
    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,
}

impl Application {
    const FIXED_STEP_DELTA_SEC: f32 = 0.01;
    const FIXED_STEP_DELTA_MS: u128 = 10; // 0.01 * 1000
    const MAX_FRAME_TIME_SEC: f32 = 0.25;

    const AVG_FRAME_TIME_SAMPLE: usize = 100;

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
            "crate1_diffuse",
        )
        .unwrap();

        let camera = OrthographicCamera::new(
            &renderer.device,
            &renderer.queue,
            Vector3::new(0.0, 0.0, 0.0),
            0.0,
            3.0,
            0.0,
            3.0,
            -1.0,
            1.0,
        );

        let object = PosWithUvBuffer::new_square(&renderer.device);

        let instances = (0..3)
            .flat_map(|y| {
                (0..3).map(move |x| {
                    let scale_factor = 1.0 - (((x * 3) + y) as f32 * 0.05);
                    Instance::new(
                        Vector3::new(x as f32, y as f32, 0.0),
                        Vector3::new(scale_factor, scale_factor, scale_factor),
                    )
                })
            })
            .collect::<Vec<_>>();

        let render_pass = RenderPass::new(
            &renderer.device,
            diffuse_texture,
            shader,
            object,
            instances,
            renderer.config.format,
            &camera,
            "crate",
        );
        renderer.render_passes.push(render_pass);

        Ok(Self {
            window,
            renderer,

            last_frame_start: Instant::now(),
            frame_counter: 0,
            frame_time_accumulator: 0.0,
            total_frame_time_ms: 0,
            avg_frame_time_ms: vec![0.0; Self::AVG_FRAME_TIME_SAMPLE],
            avg_frame_time_ms_index: 0,
        })
    }

    fn get_avg_frame_time_ms(&self) -> f32 {
        self.avg_frame_time_ms.iter().sum::<f32>() / self.avg_frame_time_ms.len() as f32
    }

    fn process_input_event(
        &self,
        event: &winit::event::WindowEvent<'_>,
        control_flow: &mut ControlFlow,
    ) {
        match event {
            WindowEvent::KeyboardInput {
                input:
                    KeyboardInput {
                        state: ElementState::Pressed,
                        virtual_keycode: Some(VirtualKeyCode::Escape),
                        ..
                    },
                ..
            } => {
                *control_flow = ControlFlow::Exit;
                debug!("Average frame time: {0}ms", self.get_avg_frame_time_ms());
            }
            _ => {}
        }
    }

    fn update(&mut self) {
        let current_frame_start = Instant::now();
        let elapsed_time = f32::min(
            Self::MAX_FRAME_TIME_SEC,
            (current_frame_start - self.last_frame_start).as_secs_f32(),
        );
        self.last_frame_start = current_frame_start;

        self.frame_time_accumulator += elapsed_time;

        while self.frame_time_accumulator >= Self::FIXED_STEP_DELTA_SEC {
            // todo: call fixed step update
            self.total_frame_time_ms += Self::FIXED_STEP_DELTA_MS;
            self.frame_time_accumulator -= Self::FIXED_STEP_DELTA_SEC;
        }

        // todo: call free time update

        // Statistics counters
        self.avg_frame_time_ms[self.avg_frame_time_ms_index] = elapsed_time;
        self.avg_frame_time_ms_index =
            (self.avg_frame_time_ms_index + 1) % Self::AVG_FRAME_TIME_SAMPLE;
        self.frame_counter += 1;
    }

    fn render(&mut self) {
        // todo: process error
        self.renderer.render().unwrap();
        self.window.request_redraw();
    }

    pub fn process_event(&mut self, event: &Event<'_, ()>, control_flow: &mut ControlFlow) {
        match event {
            Event::WindowEvent {
                ref event,
                window_id,
            } if *window_id == self.window.id() => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                WindowEvent::KeyboardInput { .. } => self.process_input_event(event, control_flow),
                WindowEvent::Resized(physical_size) => {
                    self.renderer.resize(*physical_size);
                }
                WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                    self.renderer.resize(**new_inner_size);
                }
                _ => {}
            },
            Event::MainEventsCleared => {
                self.update();
                self.render();
            }
            _ => {}
        }
    }
}
