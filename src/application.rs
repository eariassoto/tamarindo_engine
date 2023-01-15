// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::time::Instant;

use log::debug;
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{error::EngineError, render::RenderState};

pub trait ApplicationImpl {
    fn init_resources(&mut self, app: &mut Application);

    fn update(&mut self, app: &mut Application, delta_time: f32);
    fn render(&mut self, app: &mut Application);
}

pub trait KeyboardState {
    fn is_key_pressed(&self, keycode: VirtualKeyCode) -> bool;
    fn was_key_pressed_this_frame(&self, keycode: VirtualKeyCode) -> bool;
    fn was_key_released_this_frame(&self, keycode: VirtualKeyCode) -> bool;
}

// todo: remove
#[allow(dead_code)]
pub struct Application {
    window: Window,
    render_state: RenderState,

    // todo: move to input manager, do it better
    curr_frame_keys: [bool; 255],
    previous_frame_keys: [bool; 255],

    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,
}

impl KeyboardState for Application {
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
}

impl Application {
    const FIXED_STEP_DELTA_SEC: f32 = 0.01;
    const FIXED_STEP_DELTA_MS: u128 = 10; // 0.01 * 1000
    const MAX_FRAME_TIME_SEC: f32 = 0.25;

    const AVG_FRAME_TIME_SAMPLE: usize = 100;

    pub fn new(window: Window) -> Result<Self, EngineError> {
        let render_state = pollster::block_on(RenderState::new(&window));

        let curr_frame_keys = [false; 255];
        let previous_frame_keys = curr_frame_keys.clone();

        Ok(Self {
            window,
            render_state,

            curr_frame_keys,
            previous_frame_keys,

            last_frame_start: Instant::now(),
            frame_counter: 0,
            frame_time_accumulator: 0.0,
            total_frame_time_ms: 0,
            avg_frame_time_ms: vec![0.0; Self::AVG_FRAME_TIME_SAMPLE],
            avg_frame_time_ms_index: 0,
        })
    }

    pub fn load<T>(&mut self, app_impl: &mut T)
    where
        T: ApplicationImpl,
    {
        app_impl.init_resources(self);
    }

    pub fn process_event<T>(
        &mut self,
        event: &Event<'_, ()>,
        control_flow: &mut ControlFlow,
        app_impl: &mut T,
    ) where
        T: ApplicationImpl,
    {
        match event {
            Event::WindowEvent {
                ref event,
                window_id,
            } if *window_id == self.window.id() => match event {
                WindowEvent::CloseRequested => *control_flow = ControlFlow::Exit,
                WindowEvent::KeyboardInput { .. } => self.process_input_event(event, control_flow),
                WindowEvent::Resized(physical_size) => {
                    self.render_state.resize(*physical_size);
                }
                WindowEvent::ScaleFactorChanged { new_inner_size, .. } => {
                    self.render_state.resize(**new_inner_size);
                }
                _ => {}
            },
            Event::MainEventsCleared => {
                self.previous_frame_keys
                    .copy_from_slice(&self.curr_frame_keys);
                self.update(app_impl);
                self.render(app_impl);
            }
            _ => {}
        }
    }

    pub fn render_state(&self) -> &RenderState {
        &self.render_state
    }

    fn get_avg_frame_time_ms(&self) -> f32 {
        self.avg_frame_time_ms.iter().sum::<f32>() / self.avg_frame_time_ms.len() as f32
    }

    fn process_input_event(
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
                        debug!("Average frame time: {0}ms", self.get_avg_frame_time_ms());
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

    fn update<T>(&mut self, app_impl: &mut T)
    where
        T: ApplicationImpl,
    {
        let current_frame_start = Instant::now();
        let elapsed_time = f32::min(
            Self::MAX_FRAME_TIME_SEC,
            (current_frame_start - self.last_frame_start).as_secs_f32(),
        );
        self.last_frame_start = current_frame_start;

        self.frame_time_accumulator += elapsed_time;

        while self.frame_time_accumulator >= Self::FIXED_STEP_DELTA_SEC {
            // todo: call fixed step update
            self.total_frame_time_ms += Self::FIXED_STEP_DELTA_MS;
            self.frame_time_accumulator -= Self::FIXED_STEP_DELTA_SEC;
        }

        // todo: call free time update
        app_impl.update(self, elapsed_time);

        // Statistics counters
        self.avg_frame_time_ms[self.avg_frame_time_ms_index] = elapsed_time;
        self.avg_frame_time_ms_index =
            (self.avg_frame_time_ms_index + 1) % Self::AVG_FRAME_TIME_SAMPLE;
        self.frame_counter += 1;
    }

    fn render<T>(&mut self, app_impl: &mut T)
    where
        T: ApplicationImpl,
    {
        app_impl.render(self);
        self.window.request_redraw();
    }
}
