#pragma once
#include <SFML/Window/Keyboard.hpp>

#include <string_view>

namespace engine::input
{

    std::string_view keyToString(sf::Keyboard::Key key);

}  // namespace engine::input