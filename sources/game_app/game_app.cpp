// Copyright(c) 2021 Emmanuel Arias
#include "game_app.h"

#include <string>

using namespace tamarindo;

std::unique_ptr<Application> CreateApplication()
{
    return std::make_unique<GameApp>();
}

GameApp::GameApp() : m_WindowProperties("Tamarindo Engine Demo App", 800, 600)
{
    m_WindowProperties.DefaultBackground = {0.1f, 0.1f, 0.1f};
}

bool GameApp::doInitialize()
{
    std::string vertex_shader =
        "#version 460 core\n"
        "layout(location = 0) in vec3 aPos;"
        "layout(location = 1) in vec2 aUVs;"
        "out vec2 UVs;"
        "void main() {"
        "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);"
        "    UVs = aUVs;"
        "}";
    std::string fragment_shader =
        "#version 460 core\n"
        "in vec2 UVs;"
        "out vec4 FragColor;"
        "void main() {"
        "    FragColor = vec4(UVs.x, UVs.y, 0.0f, 1.0f);"
        "}";

    auto [init, shader_id] =
        ShaderProgram::createNewShader(vertex_shader, fragment_shader);
    if (!init) {
        TM_LOG_ERROR("Could not create shader");
        return false;
    }

    m_ShaderProgram = shader_id;

    /*
     * The first three parameters of the vertex are the position
     * of the vertex itself. Here we are creating a rectangle that is
     * half the size of the screen. Notice that the origin is in the center:
     *
     *   +---------------------------------+
     *   |   (-0.5,0.5)         (0.5,0,5)  |
     *   |       +-----------------+       |
     *   |       |                 |       |
     *   |       |      (0,0)      |       |
     *   |       |                 |       |
     *   |       +-----------------+       |
     *   | (-0.5,-0.5)         (0.5,-0.5)  |
     *   +---------------------------------+
     *
     * The second two parameters are the UV coordinates. The origin here is in
     * the top left corner:
     *
     *  (0,0)       (1,0)
     *     +---------+
     *     |         |
     *     |         |
     *     |         |
     *     |         |
     *     +---------+
     *  (0,1)       (1,1)
     *
     */
    Mesh rectMesh(4, 6);
    rectMesh.addVertex(0.5f, 0.5f, 0.0f, 1.0f, 0.0f);    // top right
    rectMesh.addVertex(0.5f, -0.5f, 0.0f, 1.0f, 1.0f);   // bottom right
    rectMesh.addVertex(-0.5f, -0.5f, 0.0f, 0.0f, 1.0f);  // bottom left
    rectMesh.addVertex(-0.5f, 0.5f, 0.0f, 0.0f, 0.0f);   // top left
    rectMesh.addIndex(0);                                // first triangle
    rectMesh.addIndex(1);
    rectMesh.addIndex(3);
    rectMesh.addIndex(1);  // second triangle
    rectMesh.addIndex(2);
    rectMesh.addIndex(3);

    m_RectMeshInstance = Mesh::createInstance(rectMesh);

    return true;
}

void GameApp::doTerminate()
{
    Mesh::terminateInstance(m_RectMeshInstance);
    ShaderProgram::terminateShader(m_ShaderProgram);
}

void GameApp::doUpdate(std::chrono::duration<double> delta_time) {}

void GameApp::doRender()
{
    ShaderProgram::bindShader(m_ShaderProgram);
    Mesh::renderMeshInstance(m_RectMeshInstance);
}
