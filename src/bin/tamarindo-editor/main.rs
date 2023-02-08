// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use engine_editor::EngineEditor;
use project_config::ProjectConfig;

mod engine_editor;
mod errors;
mod project_config;

fn main() -> anyhow::Result<()> {
    env_logger::init();

    let project_config = ProjectConfig::new_from_file("res/app_config.yaml")?;

    EngineEditor::new(project_config)?.run();
    Ok(())
}
