#pragma once

#include <type_traits>

#include <SFML/Graphics.hpp>

#include "engine/Rect.hpp"


namespace engine::gui
{
enum class Alignment
{
    Left                    = 0x01,
    Right                   = 0x02,
    HorizontallyCentered    = 0x04,
    Top                     = 0x10,
    Bottom                  = 0x20,
    VerticallyCentered      = 0x40
};

Alignment operator|(Alignment lhs, Alignment rhs);
Alignment operator&(Alignment lhs, Alignment rhs);
bool isBitSet(const Alignment& alignment, const Alignment& bit);

engine::Vec2f calculateAlignmentOffset(const engine::Vec2f& parentSize, const engine::Vec2f& childSize, const Alignment& alignment);
engine::Vec2f boundsToSize(const engine::FloatRect& bounds);

}  // namespace engine::gui
