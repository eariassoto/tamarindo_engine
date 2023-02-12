// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::{
    fs::File,
    io::{BufReader, Read},
    path::Path,
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
struct ProjectConfig {
    pub main_window_config: WindowConfig,
    pub cube: String,
}

pub struct Project {
    config: ProjectConfig,
}

impl Project {
    pub fn load_from_file(path: String) -> Result<Self, EditorError> {
        let path = Path::new(&path);
        if !path.is_dir() {
            return Err(EditorError::ProjectFolderNotFoundError());
        }

        let file = match File::open(path.join("project_conf.yaml").to_str().unwrap()) {
            Ok(f) => f,
            Err(e) => return Err(EditorError::OpenConfigFileError(e)),
        };

        let mut contents = String::new();
        BufReader::new(file).read_to_string(&mut contents)?;
        let config = match serde_yaml::from_str::<ProjectConfig>(&contents) {
            Ok(config) => config,
            Err(e) => return Err(EditorError::InvalidProjectConfig(e)),
        };

        Ok(Self { config })
    }

    pub fn get_window_config(&self) -> &WindowConfig {
        &self.config.main_window_config
    }

    // TODO: Fix
    pub fn load_file(&self) -> String {
        self.config.cube.to_owned()
    }
}
