// Copyright(c) 2021 Emmanuel Arias
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