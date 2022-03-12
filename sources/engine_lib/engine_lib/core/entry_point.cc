/*
 Copyright 2021-2022 Emmanuel Arias Soto

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

#include "entry_point.h"
#include "application.h"

#include <memory>
#include <iostream>

namespace tamarindo
{
int main(int argc, char* argv[])
{
    std::unique_ptr<tamarindo::Application> app = CreateApplication();

    if (!app->initialize()) {
        std::cerr << "Could not initialize application\n";
        return -1;
    }

    std::cout << "Starting application...\n";
    app->run();

    app->terminate();

    return 0;
}

}  // namespace tamarindo