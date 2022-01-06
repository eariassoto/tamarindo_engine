// Copyright (c) 2022 Emmanuel Arias
#pragma once

#include <glm/vec3.hpp>
#include <glm/mat4x4.hpp>

namespace tamarindo
{
class Transform
{
   public:
    void initialize(const glm::vec3& position, const glm::vec3& scale);

    void setPosition(const glm::vec3& position);

    void setScale(const glm::vec3& scale);

    const glm::vec3& getPosition() const { return m_Position; }
    const glm::vec3& getScale() const { return m_Scale; }

    const glm::mat4& getTransformMatrix() const;

   private:
#ifdef DEBUG
    bool md_IsInitialized = false;
#endif  // DEBUG

    glm::vec3 m_Position;

    // TODO: implement rotation

    glm::vec3 m_Scale;

    glm::mat4 m_TransformMatrix;

    void calculateTransformMatrix();
};

}  // namespace tamarindo
