// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::time::Instant;

use cgmath::Vector3;
use log::debug;
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{
    camera::OrthographicCamera,
    render::{pipeline::new_pipeline, RenderState},
    resources::{
        model::{
            self, DrawInstancedModel, Instance, InstancedModel, Material, Mesh, ModelVertex, Vertex,
        },
        shader::Shader,
        texture::Texture,
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

// todo: remove
#[allow(dead_code)]
pub struct Application {
    window: Window,
    render_state: RenderState,
    // todo: fix this
    camera: OrthographicCamera,
    camera_bind_group: wgpu::BindGroup,
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
        let render_state = pollster::block_on(RenderState::new(&window));
        let shader = Shader::new(
            "crate_box",
            include_str!("../res/shaders/shader.wgsl"),
            &render_state,
        );

        // todo: handle this error
        let diffuse_texture = Texture::new_from_bytes(
            &render_state.device,
            &render_state.queue,
            include_bytes!("../res/img/3crates/crate1/crate1_diffuse.png"),
            "crate1_diffuse",
        )
        .unwrap();

        let camera = OrthographicCamera::new(
            &render_state.device,
            Vector3::new(0.0, 0.0, 0.0),
            0.0,
            3.0,
            0.0,
            3.0,
            -1.0,
            1.0,
        );

        let square_mesh_vert = ModelVertex::from_raw_data(SQUARE_VERTICES);
        let square_mesh = Mesh::new(
            &render_state.device,
            "crate_square",
            &square_mesh_vert,
            SQUARE_INDICES,
            0,
        );
        let square_mat = Material::new(&render_state.device, "crate_square", diffuse_texture);
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

        let model = InstancedModel::new(&render_state.device, square_mesh, square_mat, instances);

        let (camera_bind_group_layout, camera_bind_group) =
            camera.new_bind_group(&render_state.device);

        let bind_group_layouts = &[
            &model.get_bind_group_layouts()[..],
            &[&camera_bind_group_layout],
        ]
        .concat();

        let vertex_buffer_layouts = &[model::ModelVertex::desc(), Instance::desc()];
        let pipeline = new_pipeline(
            &render_state,
            "crate",
            vertex_buffer_layouts,
            bind_group_layouts,
            &shader,
        );

        Ok(Self {
            window,
            render_state,
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
        //self.render_state.render().unwrap();

        // todo: catch this error and return custom error enum
        let output = self.render_state.surface.get_current_texture().unwrap();

        let mut encoder =
            self.render_state
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
            render_pass.set_bind_group(1, &self.camera_bind_group, &[]);
            // model
            render_pass.draw_model(&self.model);
        }

        self.render_state
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
                    self.render_state.resize(*physical_size);
                }
                WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                    self.render_state.resize(**new_inner_size);
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
