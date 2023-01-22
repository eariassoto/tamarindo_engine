// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{Matrix4, Vector3};
use wgpu::util::DeviceExt;

use super::texture::{self, Texture};

#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct ModelVertex {
    position: [f32; 3],
    tex_coords: [f32; 2],
}

#[repr(C)]
#[derive(Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
pub struct InstanceRaw {
    model: [[f32; 4]; 4],
}

impl ModelVertex {
    const RAW_SIZE: usize = 5;

    fn new(v: &[f32]) -> Self {
        Self {
            position: [v[0], v[1], v[2]],
            tex_coords: [v[3], v[4]],
        }
    }

    pub fn from_raw_data(vertex_data: &[f32]) -> Vec<Self> {
        // todo: return error, no panic
        if vertex_data.is_empty() || vertex_data.len() % ModelVertex::RAW_SIZE != 0 {
            panic!("Tried to create a vertex array data with invalid data")
        }
        vertex_data
            .chunks(Self::RAW_SIZE)
            .map(|v| Self::new(v))
            .collect::<Vec<Self>>()
    }

    pub fn desc() -> wgpu::VertexBufferLayout<'static> {
        wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<Self>() as wgpu::BufferAddress,
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
        }
    }
}

pub struct Mesh {
    pub vertex_buffer: wgpu::Buffer,
    pub index_buffer: wgpu::Buffer,
    pub num_indices: u32,
}

impl Mesh {
    pub fn new(device: &wgpu::Device, vertex_data: &[ModelVertex], index_data: &[u16]) -> Self {
        let vertex_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Vertex Buffer"),
            contents: bytemuck::cast_slice(vertex_data),
            usage: wgpu::BufferUsages::VERTEX,
        });

        let index_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Index Buffer"),
            contents: bytemuck::cast_slice(index_data),
            usage: wgpu::BufferUsages::INDEX,
        });
        let num_indices = index_data.len() as u32;

        Self {
            vertex_buffer,
            index_buffer,
            num_indices,
        }
    }
}

pub struct Material {
    pub name: String,
    pub diffuse_texture: texture::Texture,
}

impl Material {
    pub fn new(name: &str, diffuse_texture: Texture) -> Self {
        let name = String::from(name);
        Self {
            name,
            diffuse_texture,
        }
    }
}

// todo: remove
#[allow(dead_code)]
pub struct Instance {
    position: Vector3<f32>,
    scale: Vector3<f32>,
    pub raw: InstanceRaw,
}

impl Instance {
    pub fn new(position: Vector3<f32>, scale: Vector3<f32>) -> Self {
        let raw = InstanceRaw {
            model: (Matrix4::from_translation(position)
                * Matrix4::from_nonuniform_scale(scale.x, scale.y, scale.z))
            .into(),
        };
        Self {
            position,
            scale,
            raw,
        }
    }

    pub fn desc() -> wgpu::VertexBufferLayout<'static> {
        use std::mem;
        wgpu::VertexBufferLayout {
            array_stride: mem::size_of::<InstanceRaw>() as wgpu::BufferAddress,
            // We need to switch from using a step mode of Vertex to Instance
            // This means that our shaders will only change to use the next
            // instance when the shader starts processing a new instance
            step_mode: wgpu::VertexStepMode::Instance,
            attributes: &[
                // A mat4 takes up 4 vertex slots as it is technically 4 vec4s. We need to define a slot
                // for each vec4. We'll have to reassemble the mat4 in
                // the shader.
                wgpu::VertexAttribute {
                    offset: 0,
                    // While our vertex shader only uses locations 0, and 1 now, in later tutorials we'll
                    // be using 2, 3, and 4, for Vertex. We'll start at slot 5 not conflict with them later
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
        }
    }
}

pub struct Model {
    pub meshes: Vec<Mesh>,
    pub materials: Vec<Material>,
}

impl Model {
    pub fn new(mesh: Mesh, mat: Material) -> Self {
        Self {
            meshes: vec![mesh],
            materials: vec![mat],
        }
    }
}

pub struct InstancedModel {
    pub model: Model,
    pub instance_buffer: wgpu::Buffer,
    pub num_instances: usize,
}

impl InstancedModel {
    pub fn new(device: &wgpu::Device, mesh: Mesh, mat: Material, instances: Vec<Instance>) -> Self {
        let model = Model::new(mesh, mat);

        let instance_data = instances.iter().map(|x| x.raw).collect::<Vec<_>>();
        let instance_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Instance Buffer"),
            contents: bytemuck::cast_slice(&instance_data),
            usage: wgpu::BufferUsages::VERTEX,
        });
        let num_instances = instances.len();
        Self {
            model,
            instance_buffer,
            num_instances,
        }
    }
}

pub trait DrawModel<'a> {
    fn draw_model_instanced(
        &mut self,
        model: &'a Model,
        instance_buffer: &'a wgpu::Buffer,
        num_instances: usize,
    );
}

impl<'a, 'b> DrawModel<'b> for wgpu::RenderPass<'a>
where
    'b: 'a,
{
    fn draw_model_instanced(
        &mut self,
        model: &'a Model,
        instance_buffer: &'a wgpu::Buffer,
        num_instances: usize,
    ) {
        // texture
        self.set_bind_group(1, &model.materials[0].diffuse_texture.bind_group, &[]);

        self.set_vertex_buffer(0, model.meshes[0].vertex_buffer.slice(..));
        self.set_vertex_buffer(1, instance_buffer.slice(..));

        self.set_index_buffer(
            model.meshes[0].index_buffer.slice(..),
            wgpu::IndexFormat::Uint16,
        );

        self.draw_indexed(0..model.meshes[0].num_indices, 0, 0..num_instances as _);
    }
}
