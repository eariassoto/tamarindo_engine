// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use wgpu::util::DeviceExt;

#[repr(C)]
#[derive(Copy, Clone, Debug, bytemuck::Pod, bytemuck::Zeroable)]
pub struct PosWithUvVertex {
    position: [f32; 3],
    tex_coords: [f32; 2],
}

impl PosWithUvVertex {
    const SIZE: wgpu::BufferAddress = std::mem::size_of::<Self>() as wgpu::BufferAddress;
    const DESC: wgpu::VertexBufferLayout<'static> = {
        wgpu::VertexBufferLayout {
            array_stride: Self::SIZE,
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
    };
    const RAW_SIZE: usize = 5;

    pub fn from_raw_data(data: &[f32]) -> Vec<Self> {
        if data.is_empty() || data.len() % Self::RAW_SIZE != 0 {
            panic!("Tried to create a PosWithUvVertex with invalid raw data")
        }

        data.chunks(Self::RAW_SIZE)
            .map(|v| Self {
                position: [v[0], v[1], v[2]],
                tex_coords: [v[3], v[4]],
            })
            .collect::<Vec<Self>>()
    }

    const SQUARE_VERTICES: &[f32] = &[
        1.0, 1.0, 0.0, 1.0, 0.0, // top right
        -1.0, 1.0, 0.0, 0.0, 0.0, // top left
        -1.0, -1.0, 0.0, 0.0, 1.0, // bottom left
        1.0, -1.0, 0.0, 1.0, 1.0, // bottom right
    ];
    const SQUARE_INDICES: &[u16] = &[0, 1, 2, 2, 3, 0];
}

pub struct PosWithUvBuffer {
    vertex_buffer: wgpu::Buffer,
    index_buffer: wgpu::Buffer,
    num_indices: u32,
}

impl PosWithUvBuffer {
    pub fn new_square(device: &wgpu::Device) -> Self {
        Self::new(
            &device,
            &PosWithUvVertex::from_raw_data(PosWithUvVertex::SQUARE_VERTICES),
            PosWithUvVertex::SQUARE_INDICES,
        )
    }

    pub fn new(device: &wgpu::Device, vertex_data: &[PosWithUvVertex], index_data: &[u16]) -> Self {
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
        PosWithUvVertex::DESC
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
