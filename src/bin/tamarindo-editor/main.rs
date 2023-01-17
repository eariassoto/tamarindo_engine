// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod errors;
mod project_config;

use anyhow::*;
use cgmath::Vector3;
use project_config::ProjectConfig;
use tamarindo_engine::{
    camera::{OrthographicCamera, OrthographicCameraController},
    entry_point,
    render::pass::{CreateDiffuseTexturePass, DiffuseTexturePass, RecordDiffuseTexturePass},
    resources::{Instance, InstancedModel, Material, Mesh, ModelVertex, Texture},
    Application, ApplicationImpl, WindowState,
};

// todo: fix this
const SQUARE_VERTICES: &[f32] = &[
    0.0, 1.0, 0.0, 1.0, 0.0, // top right
    0.0, 0.0, 0.0, 0.0, 0.0, // top left
    1.0, 0.0, 0.0, 0.0, 1.0, // bottom left
    1.0, 1.0, 0.0, 1.0, 1.0, // bottom right
];
const SQUARE_INDICES: &[u16] = &[0, 1, 2, 2, 3, 0];

struct EngineEditor {
    _project_config: ProjectConfig,
    // todo: fix this
    camera: Option<OrthographicCamera>,
    camera_controller: Option<OrthographicCameraController>,
    model: Option<InstancedModel>,
    pipeline: Option<DiffuseTexturePass>,
}

impl EngineEditor {
    fn new(_project_config: ProjectConfig) -> Self {
        Self {
            _project_config,
            camera: None,
            camera_controller: None,
            model: None,
            pipeline: None,
        }
    }
}

impl ApplicationImpl for EngineEditor {
    fn init_resources(&mut self, app: &mut Application) {
        let render_state = app.render_state();
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
        let camera_controller = OrthographicCameraController::new(10.0);

        let square_mesh_vert = ModelVertex::from_raw_data(SQUARE_VERTICES);
        let square_mesh = Mesh::new(device, "crate_square", &square_mesh_vert, SQUARE_INDICES, 0);
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

        self.camera = Some(camera);
        self.camera_controller = Some(camera_controller);
        self.model = Some(model);
        self.pipeline = Some(pipeline);
    }

    fn update(&mut self, app: &mut Application, delta_time: f32) {
        self.camera_controller.as_ref().unwrap().update_camera(
            &app.render_state().queue,
            app,
            delta_time,
            self.camera.as_mut().unwrap(),
        );
    }

    fn render(&mut self, app: &mut Application) {
        let render_state = app.render_state();
        // todo: catch this error and return custom error enum
        let output = render_state.surface.get_current_texture().unwrap();

        let mut encoder =
            render_state
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
            render_pass.record_pass(
                self.pipeline.as_ref().unwrap(),
                self.camera.as_ref().unwrap().bind_group(),
                self.model.as_ref().unwrap(),
            );
        }

        render_state.queue.submit(std::iter::once(encoder.finish()));
        output.present();
    }
}

fn main() -> Result<()> {
    env_logger::init();

    let project_config = ProjectConfig::new_from_file("res/app_config.yaml")?;

    let window_state = WindowState::new(
        project_config.main_window_config.name.clone(),
        project_config.main_window_config.width,
        project_config.main_window_config.height,
    )?;

    let app_impl = EngineEditor::new(project_config);
    entry_point::run::<EngineEditor>(window_state, app_impl)?;
    Ok(())
}
