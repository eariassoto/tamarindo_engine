// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{InnerSpace, Matrix4, SquareMatrix, Vector3, Zero};
use wgpu::util::DeviceExt;
use winit::event::VirtualKeyCode;

use crate::KeyboardState;

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
    pos: Vector3<f32>,
    left: f32,
    right: f32,
    bottom: f32,
    top: f32,
    z_near: f32,
    z_far: f32,
    uniform: CameraUniform,

    buffer: wgpu::Buffer,
    bind_group: wgpu::BindGroup,
}

impl OrthographicCamera {
    pub fn new(
        label: &str,
        // todo: camera and buffer should be decoupled
        device: &wgpu::Device,
        bind_group_layout: &wgpu::BindGroupLayout,
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

        let bind_group = device.create_bind_group(&wgpu::BindGroupDescriptor {
            layout: &bind_group_layout,
            entries: &[wgpu::BindGroupEntry {
                binding: 0,
                resource: buffer.as_entire_binding(),
            }],
            label: Some(format!("{}_bind_group", label).as_str()),
        });

        Self {
            // todo: validate bounds
            pos,
            left,
            right,
            bottom,
            top,
            z_near,
            z_far,

            uniform,
            buffer,
            bind_group,
        }
    }

    pub fn desc() -> wgpu::BindGroupLayoutDescriptor<'static> {
        wgpu::BindGroupLayoutDescriptor {
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
        }
    }

    pub fn bind_group(&self) -> &wgpu::BindGroup {
        &self.bind_group
    }

    pub fn move_camera_pos(&mut self, queue: &wgpu::Queue, translation: Vector3<f32>) {
        self.pos += translation;

        let view_mat = cgmath::Matrix4::from_translation(self.pos)
            .invert()
            .unwrap();
        let projection_mat = cgmath::ortho(
            self.left,
            self.right,
            self.bottom,
            self.top,
            self.z_near,
            self.z_far,
        );
        self.uniform = CameraUniform::new(OPENGL_TO_WGPU_MATRIX * projection_mat * view_mat);

        queue.write_buffer(&self.buffer, 0, bytemuck::cast_slice(&[self.uniform]));
    }
}

pub struct OrthographicCameraController {
    speed: f32,
}

impl OrthographicCameraController {
    pub fn new(speed: f32) -> Self {
        Self { speed }
    }

    pub fn update_camera<T>(
        &self,
        queue: &wgpu::Queue,
        keyboard_state: &T,
        delta_time: f32,
        camera: &mut OrthographicCamera,
    ) where
        T: KeyboardState,
    {
        let mut cam_mov = Vector3::new(0.0, 0.0, 0.0);

        if keyboard_state.is_key_pressed(VirtualKeyCode::D) {
            cam_mov[0] += 1.0;
        }
        if keyboard_state.is_key_pressed(VirtualKeyCode::A) {
            cam_mov[0] -= 1.0;
        }
        if keyboard_state.is_key_pressed(VirtualKeyCode::W) {
            cam_mov[1] += 1.0;
        }
        if keyboard_state.is_key_pressed(VirtualKeyCode::S) {
            cam_mov[1] -= 1.0;
        }

        if !cam_mov.is_zero() {
            cam_mov = (self.speed * delta_time) * cam_mov.normalize();
            camera.move_camera_pos(queue, cam_mov);
        }
    }
}
