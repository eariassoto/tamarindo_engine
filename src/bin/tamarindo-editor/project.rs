// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::{
    collections::HashMap,
    fs::File,
    io::{BufReader, Read},
    path::PathBuf,
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
    main_window_config: WindowConfig,
    models: HashMap<String, String>,
    textures: HashMap<String, String>,
}

pub struct Project {
    root_path: PathBuf,
    config: ProjectConfig,
}

impl Project {
    pub fn load_from_file(path: String) -> Result<Self, EditorError> {
        let mut root_path = PathBuf::from(path);
        if !root_path.as_path().is_dir() {
            return Err(EditorError::ProjectFolderNotFoundError());
        }

        root_path.push("project_conf.yaml");
        let file = match File::open(root_path.to_str().unwrap()) {
            Ok(f) => f,
            Err(e) => return Err(EditorError::OpenConfigFileError(e)),
        };

        let mut contents = String::new();
        BufReader::new(file).read_to_string(&mut contents)?;
        let config = match serde_yaml::from_str::<ProjectConfig>(&contents) {
            Ok(config) => config,
            Err(e) => return Err(EditorError::InvalidProjectConfig(e)),
        };
        root_path.pop();

        Ok(Self { config, root_path })
    }

    pub fn get_window_config(&self) -> &WindowConfig {
        &self.config.main_window_config
    }

    // TODO: Fix
    pub fn load_file(&self) -> Result<File, EditorError> {
        let resource = match self.config.models.get("cube") {
            Some(r) => r,
            None => return Err(EditorError::ProjectResourceNotFoundError()),
        };
        let mut path = self.root_path.to_owned();
        path.push(resource);
        let file = match File::open(path.to_str().unwrap()) {
            Ok(f) => f,
            Err(_) => return Err(EditorError::ProjectResourceFileNotFoundError()),
        };
        Ok(file)
    }
}
