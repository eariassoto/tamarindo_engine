// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

pub use engine_editor::EngineEditor;
pub use project::Project;

mod engine_editor;
mod errors;
mod project;

fn main() -> anyhow::Result<()> {
    env_logger::init();

    let project = Project::load_from_file(String::from("./res"))?;
    EngineEditor::new(project)?.run();
    Ok(())
}
