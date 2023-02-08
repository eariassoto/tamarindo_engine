// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod errors;
mod project_config;

use std::{rc::Rc, time::Instant};

use cgmath::{Point3, Vector3};
use errors::EditorError;
use project_config::ProjectConfig;
use tamarindo_engine::{
    assets_bank::AssetsBank,
    camera::PerspectiveCamera,
    input::{InputManager, KeyboardState},
    instance::Instance,
    RenderState,
};
use ulid::Ulid;
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
    render_state: Rc<RenderState>,
    bank: AssetsBank,
    input_manager: InputManager,

    // camera: PerspectiveCamera,
    // camera_controller: OrthographicCameraController,
    texture_id: Ulid,
    camera_id: Ulid,
    mesh_id: Ulid,
    pipeline_id: Ulid,
    instance_id: Ulid,

    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,
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

        let render_state = Rc::new(pollster::block_on(RenderState::new(&window)));

        let input_manager = InputManager::new();

        let mut bank = AssetsBank::new(Rc::clone(&render_state));

        let texture_id = bank
            .register_texture(include_bytes!(
                "../../../res/img/3crates/crate1/crate1_diffuse.png"
            ))
            .unwrap();

        // let camera_controller = OrthographicCameraController::new(10.0);
        let aspect_ratio: f32 = project_config.main_window_config.width as f32
            / project_config.main_window_config.width as f32;
        let camera = PerspectiveCamera::new(
            Point3::new(0.0, 5.0, 5.0),
            Point3::new(0.0, 0.0, 0.0),
            aspect_ratio,
            75.0,
            0.1,
            100.0,
        );
        let camera_id = bank.register_camera(&camera).unwrap();

        let mesh_id = bank
            .register_mesh(&project_config.vertex_data, &project_config.index_data)
            .unwrap();

        let pipeline_id = bank.register_diffuse_pipeline().unwrap();

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
        let instance_id = bank.register_instances(instances).unwrap();

        Ok(Self {
            _project_config: project_config,
            event_loop: Some(event_loop),
            window,
            input_manager,
            render_state,
            bank,

            // camera,
            // camera_controller,
            texture_id,
            camera_id,
            mesh_id,
            pipeline_id,
            instance_id,

            last_frame_start: Instant::now(),
            frame_counter: 0,
            frame_time_accumulator: 0.0,
            total_frame_time_ms: 0,
            avg_frame_time_ms: vec![0.0; Self::AVG_FRAME_TIME_SAMPLE],
            avg_frame_time_ms_index: 0,
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
                // WindowEvent::Resized(physical_size) => {
                //     self.render_state.resize(*physical_size);
                // }
                // WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                //     self.render_state.resize(**new_inner_size);
                // }
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

        // if self
        //     .camera_controller
        //     .update_camera(&self.input_manager, elapsed_time, &mut self.camera)
        // {
        //     self.bank.update_camera(&self.camera_id, &self.camera);
        // }

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

        {
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
            render_pass.set_pipeline(self.bank.get_pipeline(&self.pipeline_id));
            // bind camera
            render_pass.set_bind_group(0, self.bank.get_camera_bind_group(&self.camera_id), &[]);

            // instance
            let instance_data = self.bank.get_instance_data(&self.instance_id);
            render_pass.set_vertex_buffer(1, instance_data.0);

            // texture
            render_pass.set_bind_group(1, self.bank.get_texture_bind_group(&self.texture_id), &[]);

            let mesh_data = self.bank.get_mesh_data(&self.mesh_id);
            render_pass.set_vertex_buffer(0, mesh_data.0);
            render_pass.set_index_buffer(mesh_data.1, wgpu::IndexFormat::Uint16);
            render_pass.draw_indexed(0..mesh_data.2, 0, 0..instance_data.1 as _);
        }

        self.render_state
            .queue
            .submit(std::iter::once(encoder.finish()));
        output.present();
        self.window.request_redraw();
    }
}
