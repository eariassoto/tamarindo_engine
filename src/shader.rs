// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use crate::RenderState;

pub struct Shader {
    module: wgpu::ShaderModule,
}

impl Shader {
    pub const VERTEX_ENTRY: &str = "vs_main";
    pub const FRAGMENT_ENTRY: &str = "fs_main";

    pub fn new(label: &str, source: &str, renderer: &RenderState) -> Self {
        let module = renderer
            .device
            .create_shader_module(wgpu::ShaderModuleDescriptor {
                label: Some(format!("{}_shader", label).as_str()),
                source: wgpu::ShaderSource::Wgsl(source.into()),
            });

        Self { module }
    }

    pub fn shader_module(&self) -> &wgpu::ShaderModule {
        &self.module
    }
}
