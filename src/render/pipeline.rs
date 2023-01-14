// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use super::shader::Shader;
use wgpu::{BindGroupLayout, VertexBufferLayout};

pub fn new_pipeline(
    device: &wgpu::Device,
    label: &str,
    vertex_buffers: &[VertexBufferLayout],
    bind_group_layouts: &[&BindGroupLayout],
    shader: &Shader,
    format: wgpu::TextureFormat,
) -> wgpu::RenderPipeline {
    let render_pipeline_layout = device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
        label: Some(format!("{}_render_pipeline_layout", label).as_str()),
        bind_group_layouts: bind_group_layouts,
        push_constant_ranges: &[],
    });

    let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
        label: Some(format!("{}_render_pipeline", label).as_str()),
        layout: Some(&render_pipeline_layout),
        vertex: wgpu::VertexState {
            module: &shader.shader_module(),
            entry_point: Shader::VERTEX_ENTRY,
            buffers: vertex_buffers,
        },
        fragment: Some(wgpu::FragmentState {
            module: &shader.shader_module(),
            entry_point: Shader::FRAGMENT_ENTRY,
            targets: &[Some(wgpu::ColorTargetState {
                format: format,
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

    render_pipeline
}
