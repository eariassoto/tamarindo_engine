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

#include "engine_lib/logging/logger.h"
#include "engine_lib/rendering/gltf_mesh.h"

#include "gtest/gtest.h"
#include "tiny_gltf.h"

#include <memory>

// TODO: Fix logging
// TODO: Check on Linux

class GLTFMeshTest : public ::testing::Test
{
   protected:
    void SetUp() override
    {
        m_Logger = std::make_unique<tamarindo::Logger>();
        m_Logger->initialize();
    }

    std::unique_ptr<tamarindo ::Logger> m_Logger;
};

TEST_F(GLTFMeshTest, Method_createGLTFMesh)
{
    tinygltf::Model model;
    tinygltf::TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromFile(
        &model, &err, &warn,
        "../../third_party/kenney/blasterkit/Models/glTF/blasterA.glb");

    ASSERT_TRUE(warn.empty());
    ASSERT_TRUE(err.empty());
    ASSERT_TRUE(ret);

    tamarindo::GLTFModel mesh(model);
    mesh.bind();
}

int main(int argc, char** argv)
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}
