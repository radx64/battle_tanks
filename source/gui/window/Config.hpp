#pragma once

#include <SFML/System/Vector2.hpp>

namespace gui::window::config
{
    
    constexpr auto RESIZE_BOX_SIZE = 20.f;

    constexpr auto HEADER_HEIGHT = 42.f;

    const auto WINDOW_BORDER_OFFSET = sf::Vector2f{4.f, 4.f};

    const auto HEADER_OFFSET = 8.f;
    const auto HEADER_BUTTON_WIDTH = 26.f;

}  // namespace gui::window::config
