// Copyright(c) 2021-2022 Emmanuel Arias
#pragma once
#include "engine_lib/core/application.h"
#include "engine_lib/core/window_manager.h"
#include "engine_lib/rendering/mesh.h"
#include "engine_lib/rendering/shader_program.h"
#include "engine_lib/world/camera.h"
#include "engine_lib/world/transform.h"

#include <memory>

using namespace tamarindo;

class GameApp : public Application
{
   public:
    GameApp();

   private:
    WindowProperties m_WindowProperties;

    OrthographicCamera m_Camera;

    Transform m_SquareMeshTransform;

    bool doInitialize() override;
    void doTerminate() override;

    void doUpdate(std::chrono::duration<double> total_time,
                  std::chrono::duration<double> delta_time) override;
    void doRender() override;

    const WindowProperties& getWindowProperties() const override
    {
        return m_WindowProperties;
    };

    ShaderProgramID m_ShaderProgram = 0;
    MeshInstanceID m_SquareMeshInstance = {};
};

std::unique_ptr<Application> CreateApplication();
