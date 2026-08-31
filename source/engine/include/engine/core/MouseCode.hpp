#pragma once

#include <cstdint>

namespace engine::core
{

enum class MouseWheel
{
    Horizontal = 0,
    Vertical = 1,
};

std::string toString(const MouseWheel mw);

enum class MouseButton
{
    // For now this is purely on what SFML provides
    Left = 0,
    Right = 1,
    Middle = 2,
    Extra1 = 3,
    Extra2 = 4,
};

std::string toString(const MouseButton mb);

}  // namespace engine::core
