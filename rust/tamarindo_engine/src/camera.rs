// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{Matrix4, Vector3, SquareMatrix, Ortho};

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
}

impl OrthographicCamera {
    pub fn new(
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
        Self {
            // todo: validate bounds
            projection_mat,
            view_mat,
            uniform,
        }
    }
}

