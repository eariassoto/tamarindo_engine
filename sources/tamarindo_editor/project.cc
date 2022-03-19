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
#include "project.h"

Project::Project(const fs::path project_path) : m_ProjectPath(project_path) {}

void Project::setName(const std::string& name) { m_Name = name; }

void Project::setWindowSize(unsigned int width, unsigned int height)
{
    if (height == 0) {
        return;
    }

    m_Width = width;
    m_Height = height;
    m_AspectRatio = (float)width / height;
}

void Project::setWindowDefaultBackground(const glm::vec4& default_background)
{
    m_DefaultBackground = default_background;
}

void Project::setMainScenePath(const std::string& main_scene_path)
{
    m_MainScenePath = m_ProjectPath / main_scene_path;
}

void Project::setDataFilePath(const std::string& data_file_path)
{
    m_DataFilePath = m_ProjectPath / data_file_path;
}

bool Project::validate()
{
    bool isValid = true;
    isValid &= !m_Name.empty();
    isValid &= !m_MainScenePath.empty();
    isValid &= !m_DataFilePath.empty();
    isValid &= m_Width > 0;
    isValid &= m_Height > 0;
    return isValid;
}