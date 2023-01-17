// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use crate::{
    camera::OrthographicCamera,
    resources::{
        shader::Shader, DrawInstancedModel, Instance, InstancedModel, ModelVertex, Texture,
    },
};

use super::RenderState;

pub struct DiffuseTexturePass {
    pub pipeline: wgpu::RenderPipeline,
    pub camera_bind_group_layout: wgpu::BindGroupLayout,
    pub diffuse_texture_bind_group_layout: wgpu::BindGroupLayout,
}

impl DiffuseTexturePass {
    pub fn new(render_state: &RenderState) -> Self {
        let device = &render_state.device;
        let camera_bind_group_layout = device.create_bind_group_layout(&OrthographicCamera::desc());
        let diffuse_texture_bind_group_layout =
            device.create_bind_group_layout(&Texture::diffuse_desc());

        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some("diffuse_texture_render_pipeline_layout"),
                bind_group_layouts: &[
                    &diffuse_texture_bind_group_layout,
                    &camera_bind_group_layout,
                ],
                push_constant_ranges: &[],
            });

        let shader = Shader::new(
            "diffuse_texture",
            include_str!("../../res/shaders/shader.wgsl"),
            &render_state,
        );
        let pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: Some("diffuse_texture_render_pipeline"),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader.shader_module(),
                entry_point: Shader::VERTEX_ENTRY,
                buffers: &[ModelVertex::desc(), Instance::desc()],
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader.shader_module(),
                entry_point: Shader::FRAGMENT_ENTRY,
                targets: &[Some(wgpu::ColorTargetState {
                    format: render_state.config.format,
                    blend: Some(wgpu::BlendState::REPLACE),
                    write_mask: wgpu::ColorWrites::ALL,
                })],
            }),
            primitive: wgpu::PrimitiveState {
                topology: wgpu::PrimitiveTopology::TriangleList,
                strip_index_format: None,
                front_face: wgpu::FrontFace::Ccw,
                cull_mode: Some(wgpu::Face::Back),
                // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                polygon_mode: wgpu::PolygonMode::Fill,
                // Requires Features::DEPTH_CLIP_CONTROL
                unclipped_depth: false,
                // Requires Features::CONSERVATIVE_RASTERIZATION
                conservative: false,
            },
            depth_stencil: None,
            multisample: wgpu::MultisampleState {
                count: 1,
                mask: !0,
                alpha_to_coverage_enabled: false,
            },
            multiview: None,
        });

        Self {
            pipeline,
            camera_bind_group_layout,
            diffuse_texture_bind_group_layout,
        }
    }
}

pub trait CreateDiffuseTexturePass {
    fn create_diffuse_texture_pass(&self) -> DiffuseTexturePass;
}

impl CreateDiffuseTexturePass for RenderState {
    fn create_diffuse_texture_pass(&self) -> DiffuseTexturePass {
        DiffuseTexturePass::new(self)
    }
}

pub trait RecordDiffuseTexturePass<'a> {
    fn record_pass(
        &mut self,
        pass: &'a DiffuseTexturePass,
        camera_bind_group: &'a wgpu::BindGroup,
        model: &'a InstancedModel,
    );
}

impl<'a, 'b> RecordDiffuseTexturePass<'b> for wgpu::RenderPass<'a>
where
    'b: 'a,
{
    fn record_pass(
        &mut self,
        pass: &'a DiffuseTexturePass,
        camera_bind_group: &'a wgpu::BindGroup,
        model: &'a InstancedModel,
    ) {
        self.set_pipeline(&pass.pipeline);
        // camera
        self.set_bind_group(1, camera_bind_group, &[]);
        // model
        self.draw_model(model);
    }
}
