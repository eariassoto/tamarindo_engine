// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use log::error;
use tamarindo_engine::{Application, ApplicationNewError};

fn main() {
    env_logger::init();

    match Application::new_from_config_str(include_str!("../res/app_config.yaml")) {
        Ok(app) => app.start(),
        Err(error) => match error {
            ApplicationNewError::InvalidConfig(_) => {
                // todo: parse serde yaml internal errors
                error!("Could not parse the application configuration.")
            }
            // todo: parse other errors
        },
    }
}
