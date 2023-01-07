// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use super::buffer::Vertex;
use wgpu::util::DeviceExt;

const SQUARE_VERTICES: &[Vertex] = &[
    Vertex::new([1.0, 1.0, 0.0], [1.0, 0.0]),   // top right
    Vertex::new([-1.0, 1.0, 0.0], [0.0, 0.0]),  // top left
    Vertex::new([-1.0, -1.0, 0.0], [0.0, 1.0]), // bottom left
    Vertex::new([1.0, -1.0, 0.0], [1.0, 1.0]),  // bottom right
];
const SQUARE_INDICES: &[u16] = &[0, 1, 2, 2, 3, 0];

pub struct Shape {
    vertex_buffer: wgpu::Buffer,
    index_buffer: wgpu::Buffer,
    num_indices: u32,
}

impl Shape {
    pub fn new_square(device: &wgpu::Device) -> Self {
        Self::new(&device, SQUARE_VERTICES, SQUARE_INDICES)
    }

    fn new(device: &wgpu::Device, vertex_data: &[Vertex], index_data: &[u16]) -> Self {
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

    pub fn vertex_buffer_layout<'a>() -> wgpu::VertexBufferLayout<'a> {
        Vertex::DESC
    }

    pub fn vertex_buffer_slice(&self) -> wgpu::BufferSlice {
        self.vertex_buffer.slice(..)
    }

    pub fn index_buffer_slice(&self) -> wgpu::BufferSlice {
        self.index_buffer.slice(..)
    }

    pub fn num_indices(&self) -> u32 {
        self.num_indices
    }
}
