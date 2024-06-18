#include <iostream>

#include "Application.hpp"

int main()
{
    try
    {
        gui_sandbox::Application app;
        app.init();
        return app.run();
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << '\n';
    }
}
