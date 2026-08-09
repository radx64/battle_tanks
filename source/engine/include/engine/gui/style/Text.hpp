#pragma once

#include <cstdint>
#include <string_view>

#include <SFML/Graphics/Color.hpp>

namespace engine::gui::style
{

struct Text
{
    std::string_view fontName;
    uint32_t size = 14;
    sf::Color color;
};

}  // engine::gui::style
