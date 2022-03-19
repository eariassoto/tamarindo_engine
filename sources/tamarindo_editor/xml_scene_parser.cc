/*
 Copyright 2022 Emmanuel Arias Soto

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
#include "xml_parser.h"

#include "engine_lib/world/camera.h"
#include "engine_lib/rendering/geometry_generator.h"

#include "glm/glm.hpp"
#include "pugixml.hpp"

#include <cstring>

namespace xmlparser
{
namespace
{
glm::vec3 parseVec3(const pugi::xml_node& node)
{
    float x = node.child("x").text().as_float();
    float y = node.child("y").text().as_float();
    float z = node.child("z").text().as_float();
    return glm::vec3(x, y, z);
}

void attachCameraControllerFromNode(PerspectiveCamera* camera,
                                    const pugi::xml_node& node)
{
    float sphere_radius = node.child("sphere_radius").text().as_float();
    float speed_across_radius =
        node.child("speed_across_radius").text().as_float();
    float speed_across_sphere =
        node.child("speed_across_sphere").text().as_float();
    glm::vec3 initial_position = parseVec3(node.child("initial_position"));

    auto camera_controller = std::make_unique<SphericalCameraController>(
        sphere_radius, speed_across_radius, speed_across_sphere,
        initial_position);
    camera->attachController(std::move(camera_controller));
}

std::unique_ptr<ICamera> createCameraFromNode(const pugi::xml_node& node)
{
    auto camera_type = node.attribute("type").value();
    if (strcmp(camera_type, "perspective") == 0) {
        glm::vec3 initial_camera_pos = parseVec3(node.child("position"));

        float fov = glm::radians(node.child("fov").text().as_float());
        float z_near = node.child("z_near").text().as_float();
        float z_far = node.child("z_far").text().as_float();

        // TODO: get aspect ratio from Application
        auto camera = std::make_unique<PerspectiveCamera>(
            fov, 960.f / 540, z_near, z_far, initial_camera_pos);

        // TODO: filter per type
        pugi::xml_node controller_node = node.child("controller");
        if (controller_node) {
            attachCameraControllerFromNode(camera.get(), controller_node);
        }

        return camera;
    }
    return nullptr;
}

std::unique_ptr<GameObject> createGameObjectFromNode(const pugi::xml_node& node)
{
    // TODO: remove geometry generator dependency
    std::unique_ptr<Mesh> cube_mesh = GeometryGenerator::createCube();
    cube_mesh->getMaterial(0)->setColor(Color(0, 155, 72));
    cube_mesh->getMaterial(1)->setColor(Color(255, 255, 255));
    cube_mesh->getMaterial(2)->setColor(Color(183, 18, 52));
    cube_mesh->getMaterial(3)->setColor(Color(255, 213, 0));
    cube_mesh->getMaterial(4)->setColor(Color(0, 70, 173));
    cube_mesh->getMaterial(5)->setColor(Color(255, 88, 0));

    pugi::xml_node transform_node = node.child("transform");
    glm::vec3 pos = parseVec3(transform_node.child("position"));
    glm::vec3 scale = parseVec3(transform_node.child("scale"));

    auto game_object = std::make_unique<GameObject>(Transform(pos, scale),
                                                    std::move(cube_mesh));
    return game_object;
}

std::unique_ptr<tamarindo::Scene> parseSceneFromDoc(
    const pugi::xml_document& doc)
{
    // TODO: Consider create validator function
    pugi::xml_node scene_node = doc.child("scene");
    if (!scene_node) {
        return nullptr;
    }

    auto scene = std::make_unique<Scene>();
    pugi::xml_node camera_node = scene_node.child("camera");
    if (camera_node) {
        // TODO: handle errors
        scene->setMainCamera(std::move(createCameraFromNode(camera_node)));
    }

    pugi::xml_node go_node = scene_node.child("game_object");
    if (go_node) {
        // TODO: handle errors
        scene->setGameObject(std::move(createGameObjectFromNode(go_node)));
    }

    return scene;
}

}  // namespace

std::unique_ptr<tamarindo::Scene> parseSceneFromStrBuffer(const std::string& xml_doc)
{
    pugi::xml_document doc;
    pugi::xml_parse_result doc_loaded =
        doc.load_buffer(xml_doc.c_str(), xml_doc.size());
    if (!doc_loaded) {
        return nullptr;
    }
    return parseSceneFromDoc(doc);
}

std::unique_ptr<Scene> parseSceneFromFile(const std::string& file_path) {
    pugi::xml_document doc;

    pugi::xml_parse_result doc_loaded = doc.load_file(file_path.c_str());

    if (!doc_loaded) {
        return nullptr;
    }
    return parseSceneFromDoc(doc);
}

}  // namespace xmlsceneparser
