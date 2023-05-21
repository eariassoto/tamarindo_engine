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

#include "tamarindo_editor/application.h"

#include "glad/glad.h"
#include "GLFW/glfw3.h"

namespace tamarindo
{

namespace
{
void glfwErrorCb(int error, const char* desc)
{
    TM_LOG_ERROR("GLFW error {}: {}", error, desc);
}

GLFWwindow* initializeGlfwWithWindow(int width, int height,
                                     const std::string& title)
{
    if (GLFW_FALSE == glfwInit()) {
        return nullptr;
    }
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    glfwWindowHint(GLFW_SAMPLES, 4);

    GLFWwindow* window =
        glfwCreateWindow(width, height, title.c_str(), NULL, NULL);

    if (window == NULL) {
        TM_LOG_ERROR("Failed to create GLFW window");
        return nullptr;
    }

    glfwSetErrorCallback(glfwErrorCb);

    glfwMakeContextCurrent(window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        TM_LOG_ERROR("Failed to initialize GLAD");
        return false;
    }

    glViewport(0, 0, width, height);

    glEnable(GL_DEPTH_TEST);

    TM_LOG_TRACE("Window Manager initialized");

    return window;
}

}  // namespace

bool Application::isRunning() const
{
    return m_IsRunning && !glfwWindowShouldClose(m_Window);
}

bool Application::initialize()
{
    m_Logger.initialize();

    TM_LOG_DEBUG("Initializing application");

    m_Window = initializeGlfwWithWindow(960, 540, "Tamarindo Editor");

    m_InputManager = std::make_unique<InputManager>(m_Window);

    glfwSetWindowUserPointer(m_Window, this);
    glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode,
                                    int action, int mods) {
        Application* app = (Application*)glfwGetWindowUserPointer(window);
        app->m_InputManager->keyCallback(key, scancode, action, mods);
    });

    m_SceneRenderer = std::make_unique<SceneRenderer>(m_Window);
    m_ImGuiRenderer = std::make_unique<ImGuiRenderer>(m_Window);

    return true;
}

void Application::run()
{
    Timer timer;
    const std::array<float, 4>& default_bg = {0.1f, 0.1f, 0.1f, 1.0f};

    while (isRunning()) {
        glfwPollEvents();
        m_InputManager->startFrame();

        timer.startFrame();

        // TODO: consider order
        m_MainScene->update(timer);
        m_SceneRenderer->update(timer);
        m_ImGuiRenderer->update(timer);

        glClearColor(default_bg[0], default_bg[1], default_bg[2], 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        m_SceneRenderer->render();
        m_ImGuiRenderer->render();

        m_InputManager->finishFrame();

        glfwSwapBuffers(m_Window);

        timer.endFrame();
    }
}

void Application::stop() { m_IsRunning = false; }

Scene* Application::getMainScene() const { return m_MainScene.get(); }

void Application::terminate()
{
    TM_LOG_DEBUG("Terminating application");
    if (m_MainScene != nullptr) {
        m_MainScene->terminate();
        m_MainScene.reset();
    }

    m_ImGuiRenderer->terminate();
    m_SceneRenderer->terminate();

    glfwTerminate();

    // Terminate internal modules here
    m_Logger.terminate();
}

// bool Application::doInitialize()
// {
//     SphericalCameraParams sphericalCameraParams;
//     sphericalCameraParams.FovAngleInRad = glm::radians(45.f);
//     // TODO: Get from Application
//     sphericalCameraParams.AspectRatio = 960.f / 540;
//     sphericalCameraParams.ZNear = 0.1f;
//     sphericalCameraParams.ZFar = 100.f;
//     sphericalCameraParams.SpherePosition = glm::vec3(0.0f);
//     sphericalCameraParams.SphereRadius = 5.f;
//     sphericalCameraParams.MoveRadiusUnitsPerSecond = 5.f;
//     sphericalCameraParams.SpeedRadsPerSec = glm::radians(50.f);

//     GLTFGameObjectDesc desc;
//     desc.ModelPath = "../../third_party/kenney/carkit/Models/glTF/race.glb";

//     std::unique_ptr<GameObject> game_object =
//     GLTFGameObjectLoader::load(desc);

//     /* auto gltf_mesh = std::make_unique<GLTFModel>(model);
//      gltf_mesh->initialize();*/

//     auto camera = std::make_unique<SphericalCamera>(sphericalCameraParams);

//     /* auto cube_game_object = std::make_unique<GameObject>(
//          Transform(glm::vec3(0.f, 0.f, 0.f), glm::vec3(1.f, 1.f, 1.f)),
//          std::move(gltf_mesh));*/

//     m_MainScene = std::make_unique<Scene>();
//     m_MainScene->setGameObject(std::move(game_object));
//     m_MainScene->setCamera(std::move(camera));

//     return true;
// }

}  // namespace tamarindo
