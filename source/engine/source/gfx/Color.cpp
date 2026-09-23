#include "engine/gfx/Color.hpp"

namespace engine::gfx
{

Color::Color(std::uint8_t red, std::uint8_t green, std::uint8_t blue, std::uint8_t alpha)
    : r(red), g(green), b(blue), a(alpha)
{}

const Color Color::Black{0, 0, 0};
const Color Color::White{255, 255, 255};
const Color Color::Red{255, 0, 0};
const Color Color::Green{0, 255, 0};
const Color Color::Blue{0, 0, 255};
const Color Color::Yellow{255, 255, 0};
const Color Color::Magenta{255, 0, 255};
const Color Color::Cyan{0, 255, 255};
const Color Color::Transparent{0, 0, 0, 0};

}  // namespace engine::gfx
