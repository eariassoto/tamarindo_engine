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
    camera::{KeyboardState, OrthographicCamera, OrthographicCameraController},
    render::{
        pass::{
            CreateDiffuseTexturePass, DiffuseTexturePass, RenderPass,
        },
        RenderState,
    },
    resources::{Instance, InstancedModel, Material, Mesh, ModelVertex, Texture},
};
use winit::{
    event::{ElementState, Event, KeyboardInput, VirtualKeyCode, WindowEvent},
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

    // todo: move to input manager, do it better
    curr_frame_keys: [bool; 255],
    previous_frame_keys: [bool; 255],

    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,

    // todo: fix this
    camera: OrthographicCamera,
    _camera_controller: OrthographicCameraController,
    model: InstancedModel,
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

        let curr_frame_keys = [false; 255];
        let previous_frame_keys = curr_frame_keys.clone();

        let device: &wgpu::Device = &render_state.device;

        let pipeline = render_state.create_diffuse_texture_pass();

        // todo: handle this error
        let diffuse_texture = Texture::new_from_bytes(
            &render_state,
            &pipeline.diffuse_texture_bind_group_layout,
            include_bytes!("../../../res/img/3crates/crate1/crate1_diffuse.png"),
            "crate1_diffuse",
        )
        .unwrap();

        let camera = OrthographicCamera::new(
            "main_camera",
            device,
            &pipeline.camera_bind_group_layout,
            Vector3::new(0.0, 0.0, 0.0),
            0.0,
            3.0,
            0.0,
            3.0,
            -1.0,
            1.0,
        );
        let _camera_controller = OrthographicCameraController::new(10.0);

        let square_mesh_vert = ModelVertex::from_raw_data(&project_config.vertex_data);
        let square_mesh = Mesh::new(
            device,
            "crate_square",
            &square_mesh_vert,
            &project_config.index_data,
            0,
        );
        let square_mat = Material::new("crate_square", diffuse_texture);
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

        let model = InstancedModel::new(device, square_mesh, square_mat, instances);

        Ok(Self {
            _project_config: project_config,

            event_loop: Some(event_loop),
            window,
            render_state,

            curr_frame_keys,
            previous_frame_keys,

            last_frame_start: Instant::now(),
            frame_counter: 0,
            frame_time_accumulator: 0.0,
            total_frame_time_ms: 0,
            avg_frame_time_ms: vec![0.0; Self::AVG_FRAME_TIME_SAMPLE],
            avg_frame_time_ms_index: 0,

            camera,
            _camera_controller,
            model,
            diffuse_pass: pipeline,
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
                self.previous_frame_keys
                    .copy_from_slice(&self.curr_frame_keys);
                self.update();
                self.render();
            }
            _ => {}
        }
    }

    fn process_input_event(
        &mut self,
        event: &winit::event::WindowEvent<'_>,
        control_flow: &mut ControlFlow,
    ) {
        match event {
            WindowEvent::KeyboardInput {
                input:
                    KeyboardInput {
                        virtual_keycode: Some(virtual_keycode),
                        state,
                        ..
                    },
                ..
            } => match state {
                ElementState::Pressed => {
                    if *virtual_keycode == VirtualKeyCode::Escape {
                        *control_flow = ControlFlow::Exit;
                        log::debug!("Average frame time: {0}ms", self.get_avg_frame_time_ms());
                    } else {
                        self.curr_frame_keys[*virtual_keycode as usize] = true;
                    }
                }
                ElementState::Released => {
                    self.curr_frame_keys[*virtual_keycode as usize] = false;
                }
            },
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
        // self.camera_controller.update_camera(
        //     &self.render_state.queue,
        //     self,
        //     elapsed_time,
        //     &mut self.camera,
        // );

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
        self.diffuse_pass
            .record(&mut encoder, &view, self.camera.bind_group(), &self.model);

        self.render_state
            .queue
            .submit(std::iter::once(encoder.finish()));
        output.present();
        self.window.request_redraw();
    }
}

impl KeyboardState for EngineEditor {
    fn is_key_pressed(&self, keycode: VirtualKeyCode) -> bool {
        self.curr_frame_keys[keycode as usize]
    }

    fn was_key_pressed_this_frame(&self, keycode: VirtualKeyCode) -> bool {
        self.previous_frame_keys[keycode as usize] == false
            && self.curr_frame_keys[keycode as usize] == true
    }

    fn was_key_released_this_frame(&self, keycode: VirtualKeyCode) -> bool {
        self.previous_frame_keys[keycode as usize] == true
            && self.curr_frame_keys[keycode as usize] == false
    }
}
