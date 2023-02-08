// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{perspective, Angle, InnerSpace, Matrix4, Point3, Rad, SquareMatrix, Vector3, Zero};
use winit::event::VirtualKeyCode;

use crate::input::KeyboardState;

#[rustfmt::skip]
pub const OPENGL_TO_WGPU_MATRIX: cgmath::Matrix4<f32> = cgmath::Matrix4::new(
    1.0, 0.0, 0.0, 0.0,
    0.0, 1.0, 0.0, 0.0,
    0.0, 0.0, 0.5, 0.0,
    0.0, 0.0, 0.5, 1.0,
);

#[repr(C)]
#[derive(Debug, Copy, Clone, bytemuck::Pod, bytemuck::Zeroable)]
pub struct CameraUniform {
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
}

pub trait Camera {
    fn get_uniform(&self) -> CameraUniform;
}

struct PerspectiveCamera {
    // eye: Point3<f32>,
    // target: Point3<f32>,
    // up: Vector3<f32>,
    // aspect_ratio: f32,
    // fov_angle_radians: f32,
    // znear: f32,
    // zfar: f32,
    view_mat: Matrix4<f32>,
    projection_mat: Matrix4<f32>,
}

impl PerspectiveCamera {
    fn new(
        eye: Point3<f32>,
        target: Point3<f32>,
        aspect_ratio: f32,
        fov_angle_radians: f32,
        znear: f32,
        zfar: f32,
    ) -> Self {
        let up = Vector3::unit_y();
        let view_mat = Matrix4::look_at_rh(eye, target, up);
        let projection_mat = perspective(cgmath::Deg(fov_angle_radians), aspect_ratio, znear, zfar);

        Self {
            // eye,
            // target,
            // up: Vector3::unit_y(),
            // aspect_ratio,
            // fov_angle_radians,znear,
            // zfar
            view_mat,
            projection_mat,
        }
    }
}

impl Camera for PerspectiveCamera {
    fn get_uniform(&self) -> CameraUniform {
        CameraUniform::new(OPENGL_TO_WGPU_MATRIX * self.projection_mat * self.view_mat)
    }
}

pub struct SphereCamera {
    camera_impl: PerspectiveCamera,
}

impl SphereCamera {
    pub fn new(
        target: Point3<f32>,
        theta: Rad<f32>,
        phi: Rad<f32>,
        radius: f32,
        aspect_ratio: f32,
    ) -> Self {
        let radius_pos = radius;
        let x = radius_pos * phi.sin() * theta.cos();
        let z = radius_pos * phi.sin() * theta.sin();
        let y = radius_pos * phi.cos();
        let camera_impl =
            PerspectiveCamera::new(Point3::new(x, y, z), target, aspect_ratio, 75.0, 0.1, 100.0);

        Self { camera_impl }
    }
}

impl Camera for SphereCamera {
    fn get_uniform(&self) -> CameraUniform {
        self.camera_impl.get_uniform()
    }
}

pub struct OrthographicCamera {
    pos: Vector3<f32>,
    view_mat: Matrix4<f32>,
    projection_mat: Matrix4<f32>,
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
        Self {
            // todo: validate bounds
            pos,
            view_mat,
            projection_mat,
        }
    }

    pub fn move_camera_pos(&mut self, translation: Vector3<f32>) {
        self.pos += translation;

        self.view_mat = cgmath::Matrix4::from_translation(self.pos)
            .invert()
            .unwrap();
    }
}

impl Camera for OrthographicCamera {
    fn get_uniform(&self) -> CameraUniform {
        CameraUniform::new(OPENGL_TO_WGPU_MATRIX * self.projection_mat * self.view_mat)
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
        keyboard_state: &T,
        delta_time: f32,
        camera: &mut OrthographicCamera,
    ) -> bool
    where
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
            camera.move_camera_pos(cam_mov);
        }

        !cam_mov.is_zero()
    }
}
