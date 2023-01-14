// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::time::Instant;

use cgmath::Vector3;
use log::debug;
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{
    camera::OrthographicCamera,
    render::{
        bind_group::BindGroup,
        model::{
            self, DrawInstancedModel, Instance, InstancedModel, Material, Mesh, ModelVertex, Vertex,
        },
        pipeline::new_pipeline,
        shader::Shader,
        texture::Texture,
        Renderer,
    },
    Error,
};

// todo: fix this
const SQUARE_VERTICES: &[f32] = &[
    0.0, 1.0, 0.0, 1.0, 0.0, // top right
    0.0, 0.0, 0.0, 0.0, 0.0, // top left
    1.0, 0.0, 0.0, 0.0, 1.0, // bottom left
    1.0, 1.0, 0.0, 1.0, 1.0, // bottom right
];
const SQUARE_INDICES: &[u16] = &[0, 1, 2, 2, 3, 0];

pub struct Application {
    window: Window,
    renderer: Renderer,
    // todo: fix this
    camera: OrthographicCamera,
    camera_bind_group: BindGroup,
    model: InstancedModel,
    pipeline: wgpu::RenderPipeline,

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

    pub fn new(window: Window) -> Result<Self, Error> {
        let renderer = pollster::block_on(Renderer::new(&window));
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

        // let object = PosWithUvBuffer::new_square(&renderer.device);
        let square_mesh_vert = ModelVertex::from_raw_data(SQUARE_VERTICES);
        let square_mesh = Mesh::new(
            &renderer.device,
            "crate_square",
            &square_mesh_vert,
            SQUARE_INDICES,
            0,
        );
        let square_mat = Material::new(&renderer.device, "crate_square", diffuse_texture);
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

        let model = InstancedModel::new(&renderer.device, square_mesh, square_mat, instances);

        let camera_bind_group = camera.new_bind_group(&renderer.device);

        let bind_group_layouts = &[
            &model.get_bind_group_layouts()[..],
            &[&camera_bind_group.layout],
        ]
        .concat();

        let vertex_buffers = &[model::ModelVertex::desc(), Instance::desc()];
        let pipeline = new_pipeline(
            &renderer.device,
            "crate",
            vertex_buffers,
            bind_group_layouts,
            &shader,
            renderer.config.format,
        );

        Ok(Self {
            window,
            renderer,
            camera,
            camera_bind_group,
            model,
            pipeline,

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
        //self.renderer.render().unwrap();

        // todo: catch this error and return custom error enum
        let output = self.renderer.surface.get_current_texture().unwrap();

        let mut encoder =
            self.renderer
                .device
                .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                    label: Some("render_encoder"),
                });
        {
            let view = output
                .texture
                .create_view(&wgpu::TextureViewDescriptor::default());

            let mut render_pass = encoder.begin_render_pass(&wgpu::RenderPassDescriptor {
                label: Some("Render Pass"),
                color_attachments: &[Some(wgpu::RenderPassColorAttachment {
                    view: &view,
                    resolve_target: None,
                    ops: wgpu::Operations {
                        load: wgpu::LoadOp::Clear(wgpu::Color {
                            r: 141 as f64 / 256 as f64,
                            g: 153 as f64 / 256 as f64,
                            b: 174 as f64 / 256 as f64,
                            a: 1.0,
                        }),
                        store: true,
                    },
                })],
                depth_stencil_attachment: None,
            });
            render_pass.set_pipeline(&self.pipeline);
            // camera
            render_pass.set_bind_group(1, &self.camera_bind_group.bind_group, &[]);
            // model
            render_pass.draw_model(&self.model);
        }

        self.renderer
            .queue
            .submit(std::iter::once(encoder.finish()));
        output.present();

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
