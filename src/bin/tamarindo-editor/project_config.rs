// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::{
    fs::File,
    io::{BufReader, Read},
};

use crate::errors::EditorError;
use serde::Deserialize;

#[derive(Debug, PartialEq, Deserialize)]
pub struct WindowConfig {
    pub name: String,
    pub width: u32,
    pub height: u32,
}

#[derive(Debug, PartialEq, Deserialize)]
pub struct ProjectConfig {
    pub main_window_config: WindowConfig,
    // todo: fix this
    pub vertex_data: Vec<f32>,
    pub index_data: Vec<u16>,
    pub cube: String,
}

impl ProjectConfig {
    pub fn new_from_file(file_path: &str) -> Result<Self, EditorError> {
        let file = match File::open(file_path) {
            Ok(f) => f,
            Err(e) => return Err(EditorError::OpenConfigFileError(e)),
        };

        let mut buf_reader = BufReader::new(file);
        let mut contents = String::new();
        buf_reader.read_to_string(&mut contents)?;

        Self::new_from_str(&contents)
    }

    pub fn new_from_str(config: &str) -> Result<Self, EditorError> {
        match serde_yaml::from_str::<Self>(&config) {
            Ok(config) => Ok(config),
            Err(e) => return Err(EditorError::InvalidProjectConfig(e)),
        }
    }
}
