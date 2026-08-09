#pragma once
#include <SFML/Window/Keyboard.hpp>

#include <string_view>

namespace engine::gui::keyboard
{

    std::string_view keyToString(sf::Keyboard::Key key);

}  // namespace engine::gui::keyboard
