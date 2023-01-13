// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

pub struct BindGroup {
    pub bind_group: wgpu::BindGroup,
    pub layout: wgpu::BindGroupLayout,
}
