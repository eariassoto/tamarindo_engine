// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use log::error;
use tamarindo_engine::config::ApplicationConfig;
use tamarindo_engine::{Application, ApplicationNewError};

fn main() -> Result<(), i32> {
    env_logger::init();

    let app_config = match ApplicationConfig::new_from_str(include_str!("../res/app_config.yaml")) {
        Ok(app_config) => app_config,
        Err(_) => {
            error!("Could not parse the application configuration.");
            return Err(-1);
        }
    };

    match Application::new_from_config_str(app_config) {
        Ok(app) => app.start(),
        Err(error) => match error {
            ApplicationNewError::CannotCreateWindow => {
                error!("Could not create a new graphical window.")
            }
            // todo: parse other errors
        },
    }
    Ok(())
}
