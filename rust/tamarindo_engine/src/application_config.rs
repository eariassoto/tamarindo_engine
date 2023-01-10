// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use serde::Deserialize;
use crate::Error;

#[derive(Debug, PartialEq, Deserialize)]
pub struct WindowConfig {
    pub name: String,
    pub width: u32,
    pub height: u32,
}

#[derive(Debug, PartialEq, Deserialize)]
pub struct ApplicationConfig {
    pub main_window_config: WindowConfig,    
    // todo: fix this
    pub vertex_data: Vec<f32>,
    pub index_data: Vec<u16>,
}

impl ApplicationConfig {
    pub fn new_from_str(config: &str) -> Result<Self, Error> {
        match serde_yaml::from_str::<ApplicationConfig>(&config) {
            Ok(config) => Ok(config),
            Err(e) => return Err(Error::InvalidApplicationConfig(e)),
        }
    }
}
