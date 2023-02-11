// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::{collections::BTreeMap, mem, rc::Rc};

use image::GenericImageView;
use ulid::Ulid;
use wgpu::util::DeviceExt;

use crate::{camera::Camera, instance::Instance, shader::Shader, EngineError, RenderState};

pub struct AssetsBank {
    render_state: Rc<RenderState>,
    diffuse_texture_bind_group_layout: wgpu::BindGroupLayout,
    camera_bind_group_layout: wgpu::BindGroupLayout,

    mesh_map: BTreeMap<Ulid, (wgpu::Buffer, wgpu::Buffer, u32)>,
    bind_group_map: BTreeMap<Ulid, wgpu::BindGroup>,
    camera_map: BTreeMap<Ulid, (wgpu::Buffer, wgpu::BindGroup)>,
    pipeline_map: BTreeMap<Ulid, wgpu::RenderPipeline>,
    instance_map: BTreeMap<Ulid, (wgpu::Buffer, usize)>,
}

impl AssetsBank {
    pub fn new(rs: Rc<RenderState>) -> Self {
        let diffuse_texture_bind_group_layout = rs
            .device
            .create_bind_group_layout(&Self::DIFFUSE_TEX_LAYOUT_DESC);
        let camera_bind_group_layout = rs
            .device
            .create_bind_group_layout(&Self::CAMERA_LAYPUT_DESC);

        Self {
            render_state: rs,
            diffuse_texture_bind_group_layout,
            camera_bind_group_layout,

            mesh_map: BTreeMap::new(),
            bind_group_map: BTreeMap::new(),
            camera_map: BTreeMap::new(),
            pipeline_map: BTreeMap::new(),
            instance_map: BTreeMap::new(),
        }
    }

    pub fn register_mesh(
        &mut self,
        vertex_data: &[f32],
        index_data: &[u32],
    ) -> Result<Ulid, EngineError> {
        let mesh_id = Ulid::new();
        if vertex_data.is_empty() || vertex_data.len() % Self::VERTEX_BUFFER_SIZE != 0 {
            panic!("Tried to create a vertex array data with invalid data")
        }
        let vertex_buffer =
            self.render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some(mesh_id.to_string().as_str()),
                    contents: bytemuck::cast_slice(vertex_data),
                    usage: wgpu::BufferUsages::VERTEX,
                });

        let index_buffer =
            self.render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some(mesh_id.to_string().as_str()),
                    contents: bytemuck::cast_slice(index_data),
                    usage: wgpu::BufferUsages::INDEX,
                });
        let num_indices = index_data.len() as u32;

        self.mesh_map
            .insert(mesh_id, (vertex_buffer, index_buffer, num_indices));
        Ok(mesh_id)
    }

    pub fn register_camera(&mut self, camera: &impl Camera) -> Result<Ulid, EngineError> {
        let camera_id = Ulid::new();
        let buffer =
            self.render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Camera Buffer"),
                    contents: bytemuck::cast_slice(&[camera.get_uniform()]),
                    usage: wgpu::BufferUsages::UNIFORM | wgpu::BufferUsages::COPY_DST,
                });
        let bind_group = self
            .render_state
            .device
            .create_bind_group(&wgpu::BindGroupDescriptor {
                layout: &self.camera_bind_group_layout,
                entries: &[wgpu::BindGroupEntry {
                    binding: 0,
                    resource: buffer.as_entire_binding(),
                }],
                label: Some("camera_bind_group"),
            });

        self.camera_map.insert(camera_id, (buffer, bind_group));
        Ok(camera_id)
    }

    pub fn register_texture(&mut self, texture_data: &[u8]) -> Result<Ulid, EngineError> {
        let texture_id = Ulid::new();
        let img = match image::load_from_memory(texture_data) {
            Ok(i) => i,
            Err(e) => return Err(EngineError::InvalidTextureData(e)),
        };
        let rgba = img.to_rgba8();
        let dimensions = img.dimensions();
        let size = wgpu::Extent3d {
            width: dimensions.0,
            height: dimensions.1,
            depth_or_array_layers: 1,
        };
        let texture = self
            .render_state
            .device
            .create_texture(&wgpu::TextureDescriptor {
                label: Some(texture_id.to_string().as_str()),
                size,
                mip_level_count: 1,
                sample_count: 1,
                dimension: wgpu::TextureDimension::D2,
                format: wgpu::TextureFormat::Rgba8UnormSrgb,
                usage: wgpu::TextureUsages::TEXTURE_BINDING | wgpu::TextureUsages::COPY_DST,
            });

        self.render_state.queue.write_texture(
            wgpu::ImageCopyTexture {
                aspect: wgpu::TextureAspect::All,
                texture: &texture,
                mip_level: 0,
                origin: wgpu::Origin3d::ZERO,
            },
            &rgba,
            wgpu::ImageDataLayout {
                offset: 0,
                bytes_per_row: std::num::NonZeroU32::new(4 * dimensions.0),
                rows_per_image: std::num::NonZeroU32::new(dimensions.1),
            },
            size,
        );

        let view = texture.create_view(&wgpu::TextureViewDescriptor::default());
        let sampler = self
            .render_state
            .device
            .create_sampler(&wgpu::SamplerDescriptor {
                address_mode_u: wgpu::AddressMode::ClampToEdge,
                address_mode_v: wgpu::AddressMode::ClampToEdge,
                address_mode_w: wgpu::AddressMode::ClampToEdge,
                mag_filter: wgpu::FilterMode::Linear,
                min_filter: wgpu::FilterMode::Nearest,
                mipmap_filter: wgpu::FilterMode::Nearest,
                ..Default::default()
            });

        let bind_group = self
            .render_state
            .device
            .create_bind_group(&wgpu::BindGroupDescriptor {
                layout: &self.diffuse_texture_bind_group_layout,
                entries: &[
                    wgpu::BindGroupEntry {
                        binding: 0,
                        resource: wgpu::BindingResource::TextureView(&view),
                    },
                    wgpu::BindGroupEntry {
                        binding: 1,
                        resource: wgpu::BindingResource::Sampler(&sampler),
                    },
                ],
                label: Some(texture_id.to_string().as_str()),
            });
        self.bind_group_map.insert(texture_id, bind_group);
        Ok(texture_id)
    }

    pub fn register_diffuse_pipeline(&mut self) -> Result<Ulid, EngineError> {
        let pipeline_id = Ulid::new();
        let render_pipeline_layout =
            self.render_state
                .device
                .create_pipeline_layout(&wgpu::PipelineLayoutDescriptor {
                    label: Some("diffuse_texture_render_pipeline_layout"),
                    bind_group_layouts: &[
                        &self.camera_bind_group_layout,
                        &self.diffuse_texture_bind_group_layout,
                    ],
                    push_constant_ranges: &[],
                });

        let shader_desc = Shader::new();
        let shader_module =
            self.render_state
                .device
                .create_shader_module(wgpu::ShaderModuleDescriptor {
                    label: Some("diffuse_shader"),
                    source: wgpu::ShaderSource::Wgsl(shader_desc.source_code.into()),
                });

        let vertex_buffer_layout = wgpu::VertexBufferLayout {
            array_stride: std::mem::size_of::<[f32; 5]>() as wgpu::BufferAddress,
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
        };

        let instance_buffer_layout = wgpu::VertexBufferLayout {
            array_stride: mem::size_of::<[[f32; 4]; 4]>() as wgpu::BufferAddress,
            step_mode: wgpu::VertexStepMode::Instance,
            attributes: &[
                wgpu::VertexAttribute {
                    offset: 0,
                    shader_location: 5,
                    format: wgpu::VertexFormat::Float32x4,
                },
                wgpu::VertexAttribute {
                    offset: mem::size_of::<[f32; 4]>() as wgpu::BufferAddress,
                    shader_location: 6,
                    format: wgpu::VertexFormat::Float32x4,
                },
                wgpu::VertexAttribute {
                    offset: mem::size_of::<[f32; 8]>() as wgpu::BufferAddress,
                    shader_location: 7,
                    format: wgpu::VertexFormat::Float32x4,
                },
                wgpu::VertexAttribute {
                    offset: mem::size_of::<[f32; 12]>() as wgpu::BufferAddress,
                    shader_location: 8,
                    format: wgpu::VertexFormat::Float32x4,
                },
            ],
        };

        let pipeline =
            self.render_state
                .device
                .create_render_pipeline(&wgpu::RenderPipelineDescriptor {
                    label: Some("diffuse_texture_render_pipeline"),
                    layout: Some(&render_pipeline_layout),
                    vertex: wgpu::VertexState {
                        module: &shader_module,
                        entry_point: Shader::VERTEX_ENTRY,
                        buffers: &[vertex_buffer_layout, instance_buffer_layout],
                    },
                    fragment: Some(wgpu::FragmentState {
                        module: &shader_module,
                        entry_point: Shader::FRAGMENT_ENTRY,
                        targets: &[Some(wgpu::ColorTargetState {
                            format: self.render_state.config.format,
                            blend: Some(wgpu::BlendState::REPLACE),
                            write_mask: wgpu::ColorWrites::ALL,
                        })],
                    }),
                    primitive: wgpu::PrimitiveState {
                        topology: wgpu::PrimitiveTopology::TriangleList,
                        strip_index_format: None,
                        front_face: wgpu::FrontFace::Ccw,
                        cull_mode: Some(wgpu::Face::Back),
                        // Setting this to anything other than Fill requires Features::NON_FILL_POLYGON_MODE
                        polygon_mode: wgpu::PolygonMode::Fill,
                        // Requires Features::DEPTH_CLIP_CONTROL
                        unclipped_depth: false,
                        // Requires Features::CONSERVATIVE_RASTERIZATION
                        conservative: false,
                    },
                    depth_stencil: None,
                    multisample: wgpu::MultisampleState {
                        count: 1,
                        mask: !0,
                        alpha_to_coverage_enabled: false,
                    },
                    multiview: None,
                });
        self.pipeline_map.insert(pipeline_id, pipeline);
        Ok(pipeline_id)
    }

    pub fn register_instances(&mut self, instances: Vec<Instance>) -> Result<Ulid, EngineError> {
        let instance_data = instances.iter().map(|x| x.raw).collect::<Vec<_>>();
        let instance_id = Ulid::new();
        let instance_buffer =
            self.render_state
                .device
                .create_buffer_init(&wgpu::util::BufferInitDescriptor {
                    label: Some("Instance Buffer"),
                    contents: bytemuck::cast_slice(&instance_data),
                    usage: wgpu::BufferUsages::VERTEX,
                });
        self.instance_map
            .insert(instance_id, (instance_buffer, instances.len()));
        Ok(instance_id)
    }

    pub fn get_pipeline(&self, id: &Ulid) -> &wgpu::RenderPipeline {
        self.pipeline_map.get(id).unwrap()
    }

    pub fn get_texture_bind_group(&self, id: &Ulid) -> &wgpu::BindGroup {
        self.bind_group_map.get(id).unwrap()
    }

    pub fn get_camera_bind_group(&self, id: &Ulid) -> &wgpu::BindGroup {
        &self.camera_map.get(id).unwrap().1
    }

    pub fn get_instance_data(&self, id: &Ulid) -> (wgpu::BufferSlice, usize) {
        let data = self.instance_map.get(id).unwrap();
        (data.0.slice(..), data.1)
    }

    pub fn get_mesh_data(&self, id: &Ulid) -> (wgpu::BufferSlice, wgpu::BufferSlice, u32) {
        let data = self.mesh_map.get(id).unwrap();
        (data.0.slice(..), data.1.slice(..), data.2)
    }

    pub fn update_camera(&mut self, camera_id: &Ulid, camera: &impl Camera) {
        let (buffer, _) = self.camera_map.get_mut(camera_id).unwrap();
        self.render_state.queue.write_buffer(
            &buffer,
            0,
            bytemuck::cast_slice(&[camera.get_uniform()]),
        );
    }

    const VERTEX_BUFFER_SIZE: usize = 5;
    const DIFFUSE_TEX_LAYOUT_DESC: wgpu::BindGroupLayoutDescriptor<'_> =
        wgpu::BindGroupLayoutDescriptor {
            entries: &[
                wgpu::BindGroupLayoutEntry {
                    binding: 0,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    ty: wgpu::BindingType::Texture {
                        multisampled: false,
                        view_dimension: wgpu::TextureViewDimension::D2,
                        sample_type: wgpu::TextureSampleType::Float { filterable: true },
                    },
                    count: None,
                },
                wgpu::BindGroupLayoutEntry {
                    binding: 1,
                    visibility: wgpu::ShaderStages::FRAGMENT,
                    // This should match the filterable field of the
                    // corresponding Texture entry above.
                    ty: wgpu::BindingType::Sampler(wgpu::SamplerBindingType::Filtering),
                    count: None,
                },
            ],
            label: None,
        };
    const CAMERA_LAYPUT_DESC: wgpu::BindGroupLayoutDescriptor<'_> =
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
            label: None,
        };
}
