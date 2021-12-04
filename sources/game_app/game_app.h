// Copyright(c) 2021 Emmanuel Arias
#pragma once
#include "engine_lib/core/application.h"

#include <memory>

class GameApp : public tamarindo::Application
{
    bool doInitialize() override;
    void doTerminate() override;
};

std::unique_ptr<tamarindo::Application> CreateApplication();
