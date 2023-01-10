// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod application;
mod application_config;
mod render;

pub use application::{Application, ApplicationNewError};
pub use application_config::{ApplicationConfig, ApplicationConfigNewError};
