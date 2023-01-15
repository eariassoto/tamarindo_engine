// Copyright 2023 Emmanuel Arias Soto <ariassotoemmanuel@gmail.com>. All rights
// reserved. Use of this source code is governed by the Apache-2.0 license that
// can be found in the LICENSE file.

use std::time::Instant;

use log::debug;
use winit::{event::Event, event::*, event_loop::ControlFlow, window::Window};

use crate::{error::EngineError, render::RenderState};

pub trait ApplicationImpl {
    fn new() -> Self;
    fn init_resources(&mut self, app: &mut Application);

    fn update(&mut self, app: &mut Application);
    fn render(&mut self, app: &mut Application);
}

// todo: remove
#[allow(dead_code)]
pub struct Application {
    window: Window,
    render_state: RenderState,

    // Time related structs
    last_frame_start: Instant,
    frame_counter: u64,
    frame_time_accumulator: f32,

    total_frame_time_ms: u128,
    avg_frame_time_ms: Vec<f32>,
    avg_frame_time_ms_index: usize,
}

impl Application {
    const FIXED_STEP_DELTA_SEC: f32 = 0.01;
    const FIXED_STEP_DELTA_MS: u128 = 10; // 0.01 * 1000
    const MAX_FRAME_TIME_SEC: f32 = 0.25;

    const AVG_FRAME_TIME_SAMPLE: usize = 100;

    pub fn new(window: Window) -> Result<Self, EngineError> {
        let render_state = pollster::block_on(RenderState::new(&window));

        Ok(Self {
            window,
            render_state,

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
        &self,
        event: &winit::event::WindowEvent<'_>,
        control_flow: &mut ControlFlow,
    ) {
        match event {
            WindowEvent::KeyboardInput {
                input:
                    KeyboardInput {
                        state: ElementState::Pressed,
                        virtual_keycode: Some(VirtualKeyCode::Escape),
                        ..
                    },
                ..
            } => {
                *control_flow = ControlFlow::Exit;
                debug!("Average frame time: {0}ms", self.get_avg_frame_time_ms());
            }
            _ => {}
        }
    }

    fn update<T>(&mut self, _app_impl: &mut T)
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
