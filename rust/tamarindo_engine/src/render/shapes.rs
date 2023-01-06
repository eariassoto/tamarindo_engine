// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use super::buffer::Vertex;
use wgpu::util::DeviceExt;

const TRIANGLE_VERTICES: &[Vertex] = &[
    Vertex::new([0.0, 0.5, 0.0], [1.0, 0.0, 0.0]),
    Vertex::new([-0.5, -0.5, 0.0], [0.0, 1.0, 0.0]),
    Vertex::new([0.5, -0.5, 0.0], [0.0, 0.0, 1.0]),
];

pub struct Triangle {
    vertex_buffer: wgpu::Buffer,
    num_vertices: u32,
}

impl Triangle {
    pub fn new(device: &wgpu::Device) -> Self {
        let vertex_buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Vertex Buffer"),
            contents: bytemuck::cast_slice(TRIANGLE_VERTICES),
            usage: wgpu::BufferUsages::VERTEX,
        });
        let num_vertices = TRIANGLE_VERTICES.len() as u32;

        Self {
            vertex_buffer,
            num_vertices,
        }
    }

    pub fn vertex_buffer_layout<'a>() -> wgpu::VertexBufferLayout<'a> {
        Vertex::desc()
    }

    pub fn vertex_buffer_slice(& self) -> wgpu::BufferSlice {
        self.vertex_buffer.slice(..)
    }

    pub fn num_vertices(& self) -> u32{
        self.num_vertices
    }
}
