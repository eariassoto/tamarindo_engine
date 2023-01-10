// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use tamarindo_engine::{Application, ApplicationConfig, Error};
fn main() -> Result<(), Error> {
    env_logger::init();

    let app_config = ApplicationConfig::new_from_str(include_str!("../res/app_config.yaml"))?;
    let app = Application::new_from_config_str(app_config)?;
    app.start();
    Ok(())
}
