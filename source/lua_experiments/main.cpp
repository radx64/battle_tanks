#include <sol/sol.hpp>
#include <iostream>

int main(int argc, char* argv[])
{
    (void) argc;
    (void) argv;

    sol::state lua;
    int x = 0;
    lua.set_function("beep", [&x]{ ++x; std::cout << "It beeps!" << std::endl;});
    lua.script("beep()");
    assert(x == 1);

    return 0;
}
