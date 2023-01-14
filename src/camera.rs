// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{Matrix4, SquareMatrix, Vector3};
use wgpu::util::DeviceExt;

#[rustfmt::skip]
pub const OPENGL_TO_WGPU_MATRIX: cgmath::Matrix4<f32> = cgmath::Matrix4::new(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.0, 0.0, 0.5, 1.0,
);

#[repr(C)]
#[derive(Debug, Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
struct CameraUniform {
    // We can't use cgmath with bytemuck directly so we'll have
    // to convert the Matrix4 into a 4x4 f32 array
    view_proj: [[f32; 4]; 4],
}

impl CameraUniform {
    fn new(view_proj_mat: Matrix4<f32>) -> Self {
        Self {
            view_proj: view_proj_mat.into(),
        }
    }
}

pub struct OrthographicCamera {
    projection_mat: Matrix4<f32>,
    view_mat: Matrix4<f32>,
    uniform: CameraUniform,
    buffer: wgpu::Buffer,
}

impl OrthographicCamera {
    pub fn new(
        // todo: camera and buffer should be decoupled
        device: &wgpu::Device,
        pos: Vector3<f32>,
        left: f32,
        right: f32,
        bottom: f32,
        top: f32,
        z_near: f32,
        z_far: f32,
    ) -> Self {
        let view_mat = cgmath::Matrix4::from_translation(pos).invert().unwrap();
        let projection_mat = cgmath::ortho(left, right, bottom, top, z_near, z_far);
        let uniform = CameraUniform::new(OPENGL_TO_WGPU_MATRIX * projection_mat * view_mat);

        let buffer = device.create_buffer_init(&wgpu::util::BufferInitDescriptor {
            label: Some("Camera Buffer"),
            contents: bytemuck::cast_slice(&[uniform]),
            usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
        });

        Self {
            // todo: validate bounds
            projection_mat,
            view_mat,
            uniform,
            buffer,
        }
    }

    pub fn new_bind_group(
        &self,
        device: &wgpu::Device,
    ) -> (wgpu::BindGroupLayout, wgpu::BindGroup) {
        let layout = device.create_bind_group_layout(&wgpu::BindGroupLayoutDescriptor {
            entries: &[wgpu::BindGroupLayoutEntry {
                binding: 0,
                visibility: wgpu::ShaderStages::VERTEX,
                ty: wgpu::BindingType::Buffer {
                    ty: wgpu::BufferBindingType::Uniform,
                    has_dynamic_offset: false,
                    min_binding_size: None,
                },
                count: None,
            }],
            label: Some("camera_bind_group_layout"),
        });
        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &layout,
            entries: &[wgpu::BindGroupEntry {
                binding: 0,
                resource: self.buffer.as_entire_binding(),
            }],
            label: Some("camera_bind_group"),
        });
        (layout, bind_group)
    }
}
