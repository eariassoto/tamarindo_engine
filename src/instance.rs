// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use cgmath::{Vector3, Matrix4};

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
