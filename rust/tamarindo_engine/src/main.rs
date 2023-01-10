// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use tamarindo_engine::{Application, ApplicationConfig, Error, WindowState};
fn main() -> Result<(), Error> {
    env_logger::init();

    let app_config = ApplicationConfig::new_from_str(include_str!("../res/app_config.yaml"))?;

    let window_state = WindowState::new(&app_config.main_window_config)?;
    let mut app = Application::new(app_config, window_state.window)?;
    window_state
        .event_loop
        .run(move |event, _, control_flow| app.process_event(&event, control_flow));
}
