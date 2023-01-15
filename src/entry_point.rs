// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use crate::{Application, ApplicationImpl, EngineError, WindowState};

pub fn run<T>(window_state: WindowState) -> Result<(), EngineError>
where
    T: ApplicationImpl + 'static,
{
    let mut app_impl = T::new();
    let mut app = Application::new(window_state.window)?;
    app.load(&mut app_impl);
    window_state
        .event_loop
        .run(move |event, _, control_flow| app.process_event(&event, control_flow, &mut app_impl));
}
