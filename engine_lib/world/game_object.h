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

#ifndef ENGINE_LIB_WORLD_GAME_OBJECT_H_
#define ENGINE_LIB_WORLD_GAME_OBJECT_H_

#include "engine_lib/rendering/model.h"

#include "glm/glm.hpp"
#include "glm/gtx/quaternion.hpp"

#include <memory>
#include <string>
#include <vector>

namespace tamarindo
{
class ShaderProgram;

class Transform
{
   public:
    Transform();
    Transform(const glm::vec3& position, const glm::vec3& scale);

    void setPosition(const glm::vec3& position);

    void setScale(const glm::vec3& scale);

    void setRotation(const glm::quat& rotation);

    const glm::vec3& getPosition() const;
    const glm::vec3& getScale() const;

    const glm::mat4& getMatrix() const;

   private:
    glm::vec3 m_Position;

    glm::quat m_Rotation;

    glm::vec3 m_Scale;

    glm::mat4 m_TransformMatrix;

    void calculateTransformMatrix();
};

class GameObject
{
   public:
    GameObject(/*const Transform& transform, std::unique_ptr<Model> model*/);
    GameObject(const std::string& name);

    void terminate();

    void addChild(GameObject* child);

    // private:
    std::string m_Name;
    std::vector<GameObject*> m_Children;
    Transform m_Transform;
    /*

    Transform& getTransform();
    const Transform& getTransform() const;

    bool hasModel() const;
    Model* getModel() const;

   private:
    Transform m_Transform;
    std::unique_ptr<Model> m_Model = nullptr;*/
};

}  // namespace tamarindo

#endif  // ENGINE_LIB_WORLD_GAME_OBJECT_H_