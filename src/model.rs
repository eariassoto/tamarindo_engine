// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use super::{texture::Texture, mesh::Mesh};

// todo: this should model a list of draw commands, with reference to
// resources, rather than owning them.
pub struct Model {
    pub meshes_by_material: Vec<(Texture, Vec<Mesh>)>,
}

impl Model {
    pub fn new() -> Self {
        Self {
            meshes_by_material: Vec::new(),
        }
    }
}

pub trait DrawModel<'a> {
    fn draw_model_instanced(
        &mut self,
        model: &'a Model,
        instance_buffer: &'a wgpu::Buffer,
        num_instances: usize,
    );
}

impl<'a, 'b> DrawModel<'b> for wgpu::RenderPass<'a>
where
    'b: 'a,
{
    fn draw_model_instanced(
        &mut self,
        model: &'a Model,
        instance_buffer: &'a wgpu::Buffer,
        num_instances: usize,
    ) {
        // texture
        self.set_vertex_buffer(1, instance_buffer.slice(..));

        for (texture, meshes) in model.meshes_by_material.iter() {
            self.set_bind_group(1, &texture.bind_group, &[]);

            for mesh in meshes.iter() {
                self.set_vertex_buffer(0, mesh.vertex_buffer.slice(..));
                self.set_index_buffer(mesh.index_buffer.slice(..), wgpu::IndexFormat::Uint16);
                self.draw_indexed(0..mesh.num_indices, 0, 0..num_instances as _);
            }
        }
    }
}
