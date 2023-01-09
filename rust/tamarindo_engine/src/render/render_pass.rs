// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use crate::render::TextureBindGroup;
use crate::render::PosWithUvBuffer;

pub struct RenderPass {
    bind_group: TextureBindGroup,
    object: PosWithUvBuffer,
    render_pipeline: wgpu::RenderPipeline,
}

impl RenderPass {
    pub fn new(
        device: &wgpu::Device,
        bind_group: TextureBindGroup,
        shader: wgpu::ShaderModule,
        object: PosWithUvBuffer,
        format: wgpu::TextureFormat,
        label: &str,
    ) -> Self {
        let render_pipeline_layout =
            device.create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                label: Some(format!("{}_render_pipeline_layout", label).as_str()),
                bind_group_layouts: &[&bind_group.layout],
                push_constant_ranges: &[],
            });

        let render_pipeline = device.create_render_pipeline(&wgpu::RenderPipelineDescriptor {
            label: Some(format!("{}_render_pipeline", label).as_str()),
            layout: Some(&render_pipeline_layout),
            vertex: wgpu::VertexState {
                module: &shader,
                entry_point: "vs_main",
                buffers: &[PosWithUvBuffer::vertex_buffer_layout()],
            },
            fragment: Some(wgpu::FragmentState {
                module: &shader,
                entry_point: "fs_main",
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
            multiview: None, // 5.
        });

        Self {
            bind_group,
            object,
            render_pipeline,
        }
    }

    pub fn record_render_pass(&mut self, encoder: &mut wgpu::CommandEncoder, view: wgpu::TextureView) {
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
        render_pass.set_pipeline(&self.render_pipeline);
        render_pass.set_bind_group(0, &self.bind_group.bind_group, &[]);
        render_pass.set_vertex_buffer(0, self.object.vertex_buffer_slice());
        render_pass.set_index_buffer(self.object.index_buffer_slice(), wgpu::IndexFormat::Uint16);
        render_pass.draw_indexed(0..self.object.num_indices(), 0, 0..1);
    }
}
