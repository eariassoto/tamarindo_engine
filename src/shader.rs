// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::mem;

use crate::RenderState;

pub struct ShaderDesc {
    source_code: &'static str,
    vertex_buffer_layouts: &'static [wgpu::VertexBufferLayout<'static>],
}

const SHADER_SOURCE: &str = "
// Vertex shader
struct CameraUniform {
    view_proj: mat4x4<f32>,
};

struct InstanceInput {
    @location(5) model_matrix_0: vec4<f32>,
    @location(6) model_matrix_1: vec4<f32>,
    @location(7) model_matrix_2: vec4<f32>,
    @location(8) model_matrix_3: vec4<f32>,
};

@group(0) @binding(0)
var<uniform> camera: CameraUniform;

struct VertexInput {
    @location(0) position: vec3<f32>,
    @location(1) tex_coords: vec2<f32>,
}
struct VertexOutput {
    @builtin(position) clip_position: vec4<f32>,
    @location(0) tex_coords: vec2<f32>,
};

@vertex
fn vs_main(
    model: VertexInput,
    instance: InstanceInput,
) -> VertexOutput {
    let model_matrix = mat4x4<f32>(
        instance.model_matrix_0,
        instance.model_matrix_1,
        instance.model_matrix_2,
        instance.model_matrix_3,
    );

    var out: VertexOutput;
    out.tex_coords = model.tex_coords;
    out.clip_position = camera.view_proj * model_matrix * vec4<f32>(model.position, 1.0);
    return out;
}

// Fragment shader
@group(1) @binding(0)
var t_diffuse: texture_2d<f32>;
@group(1) @binding(1)
var s_diffuse: sampler;

@fragment
fn fs_main(in: VertexOutput) -> @location(0) vec4<f32> {
    return textureSample(t_diffuse, s_diffuse, in.tex_coords);
}
";

impl ShaderDesc {
    pub const VERTEX_ENTRY: &str = "vs_main";
    pub const FRAGMENT_ENTRY: &str = "fs_main";

    const POS_UV_BUFFER_LAYOUT: wgpu::VertexBufferLayout<'_> = wgpu::VertexBufferLayout {
        array_stride: std::mem::size_of::<[f32; 5]>() as wgpu::BufferAddress,
        step_mode: wgpu::VertexStepMode::Vertex,
        attributes: &[
            wgpu::VertexAttribute {
                offset: 0,
                shader_location: 0,
                format: wgpu::VertexFormat::Float32x3,
            },
            wgpu::VertexAttribute {
                offset: std::mem::size_of::<[f32; 3]>() as wgpu::BufferAddress,
                shader_location: 1,
                format: wgpu::VertexFormat::Float32x2,
            },
        ],
    };

    const INSTANCE_BUFFER_LAYOUT: wgpu::VertexBufferLayout<'_> = wgpu::VertexBufferLayout {
        array_stride: mem::size_of::<[[f32; 4]; 4]>() as wgpu::BufferAddress,
        step_mode: wgpu::VertexStepMode::Instance,
        attributes: &[
            wgpu::VertexAttribute {
                offset: 0,
                shader_location: 5,
                format: wgpu::VertexFormat::Float32x4,
            },
            wgpu::VertexAttribute {
                offset: mem::size_of::<[f32; 4]>() as wgpu::BufferAddress,
                shader_location: 6,
                format: wgpu::VertexFormat::Float32x4,
            },
            wgpu::VertexAttribute {
                offset: mem::size_of::<[f32; 8]>() as wgpu::BufferAddress,
                shader_location: 7,
                format: wgpu::VertexFormat::Float32x4,
            },
            wgpu::VertexAttribute {
                offset: mem::size_of::<[f32; 12]>() as wgpu::BufferAddress,
                shader_location: 8,
                format: wgpu::VertexFormat::Float32x4,
            },
        ],
    };

    pub fn new_diffuse_desc() -> Self {
        Self {
            source_code: SHADER_SOURCE,
            vertex_buffer_layouts: &[Self::POS_UV_BUFFER_LAYOUT, Self::INSTANCE_BUFFER_LAYOUT],
        }
    }
}

pub struct Shader {
    shader_desc: ShaderDesc,
    module: wgpu::ShaderModule,
    frag_targets: Vec<Option<wgpu::ColorTargetState>>,
}

impl Shader {
    pub fn new(shader_desc: ShaderDesc, render_state: &RenderState) -> Self {
        let module = render_state
            .device
            .create_shader_module(wgpu::ShaderModuleDescriptor {
                label: Some("diffuse_shader"),
                source: wgpu::ShaderSource::Wgsl(shader_desc.source_code.into()),
            });
        let frag_targets = vec![Some(wgpu::ColorTargetState {
            format: render_state.config.format,
            blend: Some(wgpu::BlendState::REPLACE),
            write_mask: wgpu::ColorWrites::ALL,
        })];

        Self {
            shader_desc,
            module,
            frag_targets,
        }
    }

    pub fn get_vertex_state(&self) -> wgpu::VertexState {
        wgpu::VertexState {
            module: &self.module,
            entry_point: ShaderDesc::VERTEX_ENTRY,
            buffers: self.shader_desc.vertex_buffer_layouts,
        }
    }

    pub fn get_fragment_state(&self) -> Option<wgpu::FragmentState> {
        Some(wgpu::FragmentState {
            module: &self.module,
            entry_point: ShaderDesc::FRAGMENT_ENTRY,
            targets: &self.frag_targets,
        })
    }
}
