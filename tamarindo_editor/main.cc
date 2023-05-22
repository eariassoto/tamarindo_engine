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

#include "engine_lib/logging/logger.h"
#include "tamarindo_editor/application.h"

int main(int argc, char* argv[])
{
    {
        tamarindo::logging::ScopedSpdLogger logger;

        tamarindo::Application app;
        if (!app.initialize()) {
            TM_LOG_ERROR("Could not initialize application.", errno);
            return -1;
        }

        TM_LOG_INFO("Starting application...");
        app.run();
        app.terminate();
    }

    return 0;
}
