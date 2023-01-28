// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{Matrix4, Vector3};
use wgpu::util::DeviceExt;

use super::texture::Texture;
pub struct Mesh {
    pub vertex_buffer: wgpu::Buffer,
    pub index_buffer: wgpu::Buffer,
    pub num_indices: u32,
}

impl Mesh {
    const VERTEX_BUFFER_SIZE: usize = 5;

    pub fn new(device: &wgpu::Device, vertex_data: &[f32], index_data: &[u16]) -> Self {
        if vertex_data.is_empty() || vertex_data.len() % Self::VERTEX_BUFFER_SIZE != 0 {
            panic!("Tried to create a vertex array data with invalid data")
        }
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

pub struct Instance {
    pub raw: [[f32; 4]; 4],
}

impl Instance {
    pub fn new(position: Vector3<f32>, scale: Vector3<f32>) -> Self {
        let raw = (Matrix4::from_translation(position)
            * Matrix4::from_nonuniform_scale(scale.x, scale.y, scale.z))
        .into();
        Self { raw }
    }
}

// todo: this should model a list of draw commands, with reference to
// resources, rather than owning them.
pub struct Model {
    pub meshes_by_material: Vec<(Texture, Vec<Mesh>)>,
}

impl Model {
    pub fn new() -> Self {
        Self {
            meshes_by_material: Vec::new(),
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
        self.set_vertex_buffer(1, instance_buffer.slice(..));

        for (texture, meshes) in model.meshes_by_material.iter() {
            self.set_bind_group(1, &texture.bind_group, &[]);

            for mesh in meshes.iter() {
                self.set_vertex_buffer(0, mesh.vertex_buffer.slice(..));
                self.set_index_buffer(mesh.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
                self.draw_indexed(0..mesh.num_indices, 0, 0..num_instances as _);
            }
        }
    }
}
