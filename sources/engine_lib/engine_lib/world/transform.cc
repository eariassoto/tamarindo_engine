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

#include "transform.h"

#include "logging/logger.h"
#include "utils/macros.h"

#include "glm/ext/matrix_transform.hpp"

namespace tamarindo
{
void Transform::initialize(const glm::vec3& position, const glm::vec3& scale)
{
#ifdef DEBUG
    md_IsInitialized = true;
#endif  // DEBUG

    m_Position = position;
    m_Scale = scale;
    calculateTransformMatrix();
}

const glm::mat4& Transform::getTransformMatrix() const
{
#ifdef DEBUG
    if (!md_IsInitialized) {
        TM_BREAK();
    }
#endif  // DEBUG
    return m_TransformMatrix;
}

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

void Transform::calculateTransformMatrix()
{
    glm::mat4 scaling_matrix = glm::scale(glm::mat4(1.0), m_Scale);

    // TODO: rotation

    glm::mat4 translation_matrix = glm::translate(glm::mat4(1.0), m_Position);

    // Because GLM has column vector operation, the world matrix is defined as
    // T * R * S
    m_TransformMatrix = translation_matrix * /*rotationMatrix * */ scaling_matrix;
}

}  // namespace tamarindo
