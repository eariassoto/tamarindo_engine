// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use log::error;
use tamarindo_engine::Application;

fn main() {
    env_logger::init();

    match Application::new() {
        Ok(app) => app.start(),
        Err(_) => {
            error!("Could not create a new Application")
        }
    }
}
