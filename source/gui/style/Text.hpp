#pragma once

#include <cstdint>
#include <string_view>

#include <SFML/Graphics/Color.hpp>

namespace gui::style
{

struct Text
{
    std::string_view fontName;
    uint32_t size = 14;
    sf::Color color;
};

}  // gui::style
