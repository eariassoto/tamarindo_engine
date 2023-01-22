// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use winit::{
    event::{ElementState, KeyboardInput, VirtualKeyCode, WindowEvent},
    event_loop::ControlFlow,
};

pub trait KeyboardState {
    fn is_key_pressed(&self, keycode: VirtualKeyCode) -> bool;
    fn was_key_pressed_this_frame(&self, keycode: VirtualKeyCode) -> bool;
    fn was_key_released_this_frame(&self, keycode: VirtualKeyCode) -> bool;
    fn clear_frame_state(&mut self);
}

pub struct InputManager {
    curr_frame_keys: [bool; 255],
    previous_frame_keys: [bool; 255],
}

impl InputManager {
    pub fn new() -> Self {
        let curr_frame_keys = [false; 255];
        let previous_frame_keys = curr_frame_keys.clone();

        Self {
            curr_frame_keys,
            previous_frame_keys,
        }
    }

    pub fn process_input_event(
        &mut self,
        event: &winit::event::WindowEvent<'_>,
        control_flow: &mut ControlFlow,
    ) {
        match event {
            WindowEvent::KeyboardInput {
                input:
                    KeyboardInput {
                        virtual_keycode: Some(virtual_keycode),
                        state,
                        ..
                    },
                ..
            } => match state {
                ElementState::Pressed => {
                    if *virtual_keycode == VirtualKeyCode::Escape {
                        *control_flow = ControlFlow::Exit;
                    } else {
                        self.curr_frame_keys[*virtual_keycode as usize] = true;
                    }
                }
                ElementState::Released => {
                    self.curr_frame_keys[*virtual_keycode as usize] = false;
                }
            },
            _ => {}
        }
    }
}

impl KeyboardState for InputManager {
    fn is_key_pressed(&self, keycode: VirtualKeyCode) -> bool {
        self.curr_frame_keys[keycode as usize]
    }

    fn was_key_pressed_this_frame(&self, keycode: VirtualKeyCode) -> bool {
        self.previous_frame_keys[keycode as usize] == false
            && self.curr_frame_keys[keycode as usize] == true
    }

    fn was_key_released_this_frame(&self, keycode: VirtualKeyCode) -> bool {
        self.previous_frame_keys[keycode as usize] == true
            && self.curr_frame_keys[keycode as usize] == false
    }

    fn clear_frame_state(&mut self) {
        self.previous_frame_keys
            .copy_from_slice(&self.curr_frame_keys);
    }
}
