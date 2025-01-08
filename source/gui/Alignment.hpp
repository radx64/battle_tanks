#pragma once

#include <type_traits>

#include <SFML/Graphics.hpp>

namespace gui
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

sf::Vector2f calculateAlignmentOffset(const sf::Vector2f& size, const sf::Vector2f& bounds, const Alignment& alignment);
sf::Vector2f boundsToSize(const sf::FloatRect& bounds);

}  // namespace gui
