#pragma once

#include <cstdint>

namespace engine::gfx
{

class Color
{
public:
    Color() = default;

    Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha = 255);

    static const Color Black;
    static const Color White;
    static const Color Red; 
    static const Color Green;
    static const Color Blue;
    static const Color Yellow;
    static const Color Magenta;
    static const Color Cyan;
    static const Color Transparent;

    std::uint8_t r{0};
    std::uint8_t g{0};
    std::uint8_t b{0};
    std::uint8_t a{255};
};

}  // namespace engine::gfx
