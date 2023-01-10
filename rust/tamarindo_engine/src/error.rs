// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use thiserror::Error;
use winit::error::OsError;

#[derive(Error, Debug)]
pub enum Error {
    // ApplicationConfig
    #[error("Cannot create application config from YAML: {0}")]
    InvalidApplicationConfig(#[from] serde_yaml::Error),
    
    // Application
    #[error("Could not create a new window: {0}")]
    CreateWinitWindowError(#[from] OsError),
}
