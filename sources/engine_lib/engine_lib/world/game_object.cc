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
#include "game_object.h"

#include "logging/logger.h"
#include "rendering/shader_program.h"
#include "utils/macros.h"

#include "glm/ext/matrix_transform.hpp"

namespace tamarindo
{
Transform::Transform()
    : m_Position(glm::vec3(0.0f, 0.0f, 0.0f)),
      m_Scale(glm::vec3(1.0f, 1.0f, 1.0f)),
      m_Rotation(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
{
    calculateTransformMatrix();
}

Transform::Transform(const glm::vec3& position, const glm::vec3& scale)
    : m_Position(position),
      m_Scale(scale),
      m_Rotation(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
{
    calculateTransformMatrix();
}

const glm::vec3& Transform::getPosition() const { return m_Position; }
const glm::vec3& Transform::getScale() const { return m_Scale; }
const glm::mat4& Transform::getMatrix() const { return m_TransformMatrix; }

void Transform::setPosition(const glm::vec3& position)
{
    m_Position = position;
    calculateTransformMatrix();
}

void Transform::setScale(const glm::vec3& scale)
{
    m_Scale = scale;
    calculateTransformMatrix();
}

void Transform::setRotation(const glm::quat& rotation)
{
    m_Rotation = rotation;
    calculateTransformMatrix();
}

void Transform::calculateTransformMatrix()
{
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0), m_Scale);

    glm::mat4 rotation_matrix = glm::toMat4(m_Rotation);

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), m_Position);

    // Because GLM has column vector operation, the world matrix is defined as
    // T * R * S
    m_TransformMatrix = translation_matrix * rotation_matrix * scaling_matrix;
}

GameObject::GameObject(
    /*const Transform& transform, std::unique_ptr<Model> mesh*/)
    : m_Name("Game Object")
/*: m_Transform(transform), m_Model(std::move(mesh))*/ {}

GameObject::GameObject(const std::string& name) : m_Name{name} {}

// Transform& GameObject::getTransform() { return m_Transform; }
//
// const Transform& GameObject::getTransform() const { return m_Transform; }
//
// Model* GameObject::getModel() const { return m_Model.get(); }
//
// bool GameObject::hasModel() const { return m_Model != nullptr; }

void GameObject::terminate()
{
    for (GameObject* child : m_Children) {
        child->terminate();
        delete child;
    }
    /* if (m_Model != nullptr) {
         m_Model->terminate();
         m_Model.reset();
     }*/
}

void GameObject::addChild(GameObject* child) { m_Children.push_back(child); }

}  // namespace tamarindo
