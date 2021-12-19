// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include "engine_lib/core/application.h"
#include "engine_lib/core/window_manager.h"
#include "engine_lib/rendering/mesh.h"
#include "engine_lib/rendering/shader_program.h"

#include <memory>

class GameApp : public tamarindo::Application
{
   public:
    GameApp();

   private:
    tamarindo::WindowProperties m_WindowProperties;

    bool doInitialize() override;
    void doTerminate() override;

    void doUpdate(std::chrono::duration<double> delta_time) override;
    void doRender() override;

    const tamarindo::WindowProperties& getWindowProperties() const override
    {
        return m_WindowProperties;
    };

    tamarindo::ShaderProgramID m_ShaderProgram = 0;
    tamarindo::MeshInstanceID m_RectMeshInstance = {};
};

std::unique_ptr<tamarindo::Application> CreateApplication();
