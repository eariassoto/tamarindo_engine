// Copyright(c) 2021 Emmanuel Arias
#include "game_app.h"

std::unique_ptr<tamarindo::Application> CreateApplication()
{
    return std::make_unique<GameApp>();
}

bool GameApp::doInitialize() { return true; }

void GameApp::doTerminate() {}
