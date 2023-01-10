// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use serde::Deserialize;

#[derive(Debug, PartialEq, Deserialize)]
pub struct ApplicationConfig {
    pub app_name: String,
    pub window_width: u32,
    pub window_height: u32,
    // todo: fix this
    pub vertex_data: Vec<f32>,
    pub index_data: Vec<u16>,
}

pub enum ApplicationConfigNewError {
    InvalidConfig,
}

impl ApplicationConfig {
    pub fn new_from_str(config: &str) -> Result<Self, ApplicationConfigNewError> {
        match serde_yaml::from_str::<ApplicationConfig>(&config) {
            Ok(config) => Ok(config),
            // todo: more error validations :)
            Err(_) => return Err(ApplicationConfigNewError::InvalidConfig),
        }
    }
}
