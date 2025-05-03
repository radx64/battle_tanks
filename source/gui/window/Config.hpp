#pragma once

#include <SFML/System/Vector2.hpp>

namespace gui::window::config
{
    
    constexpr auto RESIZE_BOX_SIZE = 20.f;

    constexpr auto HEADER_HEIGHT = 40.f;

    const auto WINDOW_BORDER_OFFSET = sf::Vector2f{4.f, 4.f};

    const auto CLOSE_BUTTON_OFFSET = sf::Vector2f{8.f, 4.f};
    const auto CLOSE_BUTTON_WIDTH = 32.f;

}  // namespace gui::window::config
