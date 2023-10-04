/*
 Copyright 2021-2023 Emmanuel Arias Soto

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at

      https://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.
 */

#ifndef TAMARINDO_EDITOR_APPLICATION_H_
#define TAMARINDO_EDITOR_APPLICATION_H_

#include <memory>

#include "camera/perspective_camera.h"
#include "camera/spherical_camera_controller.h"
#include "input/keyboard.h"
#include "rendering/matrix_constant_buffer.h"
#include "rendering/model_data.h"
#include "rendering/render_state.h"
#include "rendering/shader.h"
#include "window/window.h"
#include "window/window_event_handler.h"

#include "game_data.h"
#include "transform.h"

namespace tmrd = ::tamarindo;

class Application : public tmrd::WindowEventHandler
{
   public:
    Application();
    ~Application();

    Application(const Application& other) = delete;
    Application& operator=(const Application& other) = delete;

    void Run();

   private:
    void BindScene();

    void Update(const tmrd::Timer& t);

    void Render();

    void UpdateConstantBuffer(const DirectX::XMMATRIX& matrix,
                              tmrd::MatrixConstantBuffer* buffer);

   private:
    bool is_running_ = true;

    tmrd::Keyboard keyboard_;

    // Data section

    tmrd::RenderState render_state_;

    // End data section

    std::unique_ptr<tmrd::Shader> shader_;

    GameData::SceneData scene_data_;

    std::unique_ptr<tmrd::ModelData> scene_data_buffers_;

    std::unique_ptr<tmrd::PerspectiveCamera> camera_;
    std::unique_ptr<tmrd::SphericalCameraController> camera_controller_;

    Transform cube_transform_;
    std::unique_ptr<tmrd::MatrixConstantBuffer> cube_transform_cb_;

    Transform grid_transform_;
    std::unique_ptr<tmrd::MatrixConstantBuffer> grid_transform_cb_;

    std::unique_ptr<tmrd::MatrixConstantBuffer> scene_constant_buffer_;

    virtual LRESULT HandleWindowMessage(HWND hWnd, UINT message, WPARAM wParam,
                                        LPARAM lParam) override;
};

#endif  // TAMARINDO_EDITOR_APPLICATION_H_
