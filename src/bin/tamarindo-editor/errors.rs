// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use thiserror::Error;
use winit::error::OsError;

#[derive(Error, Debug)]
pub enum EditorError {
    #[error("Could not open project config file: {0}")]
    OpenConfigFileError(#[from] std::io::Error),

    #[error("Cannot create project config from YAML: {0}")]
    InvalidProjectConfig(#[from] serde_yaml::Error),

    #[error("Could not create a new window: {0}")]
    CreateWinitWindowError(#[from] OsError),
}
