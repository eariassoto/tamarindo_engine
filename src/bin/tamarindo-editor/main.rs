// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

mod project_config;

use project_config::ProjectConfig;
use tamarindo_engine::{Application, Error, WindowState};

fn main() -> Result<(), Error> {
    env_logger::init();

    let project_config = ProjectConfig::new_from_str(include_str!("../../../res/app_config.yaml"))?;

    let window_state = WindowState::new(
        project_config.main_window_config.name,
        project_config.main_window_config.width,
        project_config.main_window_config.height,
    )?;
    let mut app = Application::new(window_state.window)?;
    window_state
        .event_loop
        .run(move |event, _, control_flow| app.process_event(&event, control_flow));
}
