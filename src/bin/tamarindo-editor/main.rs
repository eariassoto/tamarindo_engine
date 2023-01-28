// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod errors;
mod project_config;

use std::time::Instant;

use cgmath::Vector3;
use errors::EditorError;
use project_config::ProjectConfig;
use tamarindo_engine::{
    camera::{Camera, OrthographicCamera, OrthographicCameraController},
    input::{InputManager, KeyboardState},
    render::{DiffuseTexturePass, RenderPass, RenderState},
    resources::{Instance, Mesh, Model, Texture},
};
use wgpu::util::DeviceExt;
use winit::{
    event::{Event, WindowEvent},
    event_loop::{ControlFlow, EventLoop},
    window::{Window, WindowBuilder},
};

fn main() -> anyhow::Result<()> {
    env_logger::init();

    let project_config = ProjectConfig::new_from_file("res/app_config.yaml")?;

    EngineEditor::new(project_config)?.run();
    Ok(())
}

struct EngineEditor {
    _project_config: ProjectConfig,

    event_loop: Option<EventLoop<()>>,
    window: Window,
    render_state: RenderState,

    input_manager: InputManager,

    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,

    // todo: fix this
    camera: OrthographicCamera,
    camera_buffer: wgpu::Buffer,

    camera_bind_group: wgpu::BindGroup,

    camera_controller: OrthographicCameraController,
    model: Model,
    instance_buffer: wgpu::Buffer,
    num_instances: usize,
    diffuse_pass: DiffuseTexturePass,
}

impl EngineEditor {
    const FIXED_STEP_DELTA_SEC: f32 = 0.01;
    const FIXED_STEP_DELTA_MS: u128 = 10; // 0.01 * 1000
    const MAX_FRAME_TIME_SEC: f32 = 0.25;

    const AVG_FRAME_TIME_SAMPLE: usize = 100;

    pub fn new(project_config: ProjectConfig) -> Result<Self, EditorError> {
        let event_loop = EventLoop::new();
        let window_builder = WindowBuilder::new()
            .with_title(project_config.main_window_config.name.clone())
            .with_inner_size(winit::dpi::LogicalSize::new(
                project_config.main_window_config.width,
                project_config.main_window_config.height,
            ));

        let window = match window_builder.build(&event_loop) {
            Ok(w) => w,
            Err(e) => return Err(EditorError::CreateWinitWindowError(e)),
        };

        let render_state = pollster::block_on(RenderState::new(&window));

        let input_manager = InputManager::new();

        let diffuse_pass = DiffuseTexturePass::new(&render_state);

        // todo: handle this error
        let diffuse_texture = Texture::new_from_bytes(
            &render_state,
            &diffuse_pass.diffuse_texture_bind_group_layout,
            include_bytes!("../../../res/img/3crates/crate1/crate1_diffuse.png"),
            "crate1_diffuse",
        )
        .unwrap();

        let camera =
            OrthographicCamera::new(Vector3::new(0.0, 0.0, 0.0), 0.0, 3.0, 0.0, 3.0, -1.0, 1.0);

        let camera_buffer =
            render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Camera Buffer"),
                    contents: bytemuck::cast_slice(&[camera.get_uniform()]),
                    usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
                });
        let camera_bind_group = render_state
            .device
            .create_bind_group(&wgpu::BindGroupDescriptor {
                layout: &diffuse_pass.camera_bind_group_layout,
                entries: &[wgpu::BindGroupEntry {
                    binding: 0,
                    resource: camera_buffer.as_entire_binding(),
                }],
                label: Some("camera_bind_group"),
            });

        let camera_controller = OrthographicCameraController::new(10.0);

        let square_mesh = Mesh::new(
            &render_state.device,
            &project_config.vertex_data,
            &project_config.index_data,
        );
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
        let instance_data = instances.iter().map(|x| x.raw).collect::<Vec<_>>();
        let instance_buffer =
            render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Instance Buffer"),
                    contents: bytemuck::cast_slice(&instance_data),
                    usage: wgpu::BufferUsages::VERTEX,
                });
        let num_instances = instances.len();

        let mut model = Model::new();
        model
            .meshes_by_material
            .push((diffuse_texture, vec![square_mesh]));

        Ok(Self {
            _project_config: project_config,

            event_loop: Some(event_loop),
            window,
            render_state,

            input_manager,

            last_frame_start: Instant::now(),
            frame_counter: 0,
            frame_time_accumulator: 0.0,
            total_frame_time_ms: 0,
            avg_frame_time_ms: vec![0.0; Self::AVG_FRAME_TIME_SAMPLE],
            avg_frame_time_ms_index: 0,

            camera,
            camera_buffer,
            camera_bind_group,

            camera_controller,
            model,
            instance_buffer,
            num_instances,
            diffuse_pass,
        })
    }

    pub fn run(mut self) {
        let event_loop = self.event_loop.take().unwrap();
        event_loop.run(move |event, _, control_flow| self.process_event(&event, control_flow));
    }

    pub fn process_event(&mut self, event: &Event<'_, ()>, control_flow: &mut ControlFlow) {
        match event {
            Event::WindowEvent {
                ref event,
                window_id,
            } if *window_id == self.window.id() => match event {
                WindowEvent::CloseRequested => {
                    *control_flow = ControlFlow::Exit;
                    log::debug!("Average frame time: {0}ms", self.get_avg_frame_time_ms());
                }
                WindowEvent::KeyboardInput { .. } => {
                    self.input_manager.process_input_event(event, control_flow)
                }
                WindowEvent::Resized(physical_size) => {
                    self.render_state.resize(*physical_size);
                }
                WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                    self.render_state.resize(**new_inner_size);
                }
                _ => {}
            },
            Event::MainEventsCleared => {
                self.input_manager.clear_frame_state();
                self.update();
                self.render();
            }
            _ => {}
        }
    }

    fn get_avg_frame_time_ms(&self) -> f32 {
        self.avg_frame_time_ms.iter().sum::<f32>() / self.avg_frame_time_ms.len() as f32
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
        if self
            .camera_controller
            .update_camera(&self.input_manager, elapsed_time, &mut self.camera)
        {
            self.render_state.queue.write_buffer(
                &self.camera_buffer,
                0,
                bytemuck::cast_slice(&[self.camera.get_uniform()]),
            );
        }

        // Statistics counters
        self.avg_frame_time_ms[self.avg_frame_time_ms_index] = elapsed_time;
        self.avg_frame_time_ms_index =
            (self.avg_frame_time_ms_index + 1) % Self::AVG_FRAME_TIME_SAMPLE;
        self.frame_counter += 1;
    }

    fn render(&mut self) {
        // todo: catch this error and return custom error enum
        let output = self.render_state.surface.get_current_texture().unwrap();

        let mut encoder =
            self.render_state
                .device
                .create_command_encoder(&wgpu::CommandEncoderDescriptor {
                    label: Some("render_encoder"),
                });

        let view = output
            .texture
            .create_view(&wgpu::TextureViewDescriptor::default());
        self.diffuse_pass.record(
            &mut encoder,
            &view,
            &self.camera_bind_group,
            &self.model,
            &self.instance_buffer,
            self.num_instances,
        );

        self.render_state
            .queue
            .submit(std::iter::once(encoder.finish()));
        output.present();
        self.window.request_redraw();
    }
}
