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

#pragma once

#include "glm/glm.hpp"

#include <filesystem>
#include <string>

namespace fs = std::filesystem;

class Project
{
   public:
    Project() = delete;
    Project(const fs::path project_path);

    bool validate();

    void setName(const std::string& window_title);
    void setWindowSize(unsigned int width, unsigned int height);
    void setWindowDefaultBackground(const glm::vec4& default_background);
    void setMainScenePath(const std::string& path);
    void setDataFilePath(const std::string& path);

    inline const std::string& Name() const { return m_Name; }
    inline unsigned int WindowWidth() const { return m_Width; }
    inline unsigned int WindowHeight() const { return m_Height; }
    inline const glm::vec4& WindowDefaultBackground() const
    {
        return m_DefaultBackground;
    }

    inline fs::path ProjectPath() const
    {
        return m_ProjectPath;
    }
    inline fs::path MainScenePath() const
    {
        return m_ProjectPath / m_MainScenePath;
    }
    inline fs::path DataFilePath() const
    {
        return m_ProjectPath / m_DataFilePath;
    }

   private:
    std::string m_Name;
    unsigned int m_Width;
    unsigned int m_Height;
    float m_AspectRatio;
    glm::vec4 m_DefaultBackground = {0.0f, 0.0f, 0.0f, 1.0f};

    fs::path m_ProjectPath;
    fs::path m_MainScenePath;
    fs::path m_DataFilePath;
};
