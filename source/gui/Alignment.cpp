#include <gui/Alignment.hpp>

namespace gui
{

Alignment operator|(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) |
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

Alignment operator&(Alignment lhs, Alignment rhs) 
{
    return static_cast<Alignment>(static_cast<std::underlying_type_t<Alignment>>(lhs) &
        static_cast<std::underlying_type_t<Alignment>>(rhs));
}

bool isBitSet(const Alignment& alignment, const Alignment& bit)
{
    return ((alignment & bit) == bit);
}

sf::Vector2f calculateAlignmentOffset(const sf::Vector2f& parentSize, const sf::Vector2f& childSize, const Alignment& alignment)
{
    sf::Vector2f offset {0.0f, 0.0f};

    if (isBitSet(alignment, Alignment::Left))
    {
        offset.x = 0;
    }
    else if (isBitSet(alignment, Alignment::HorizontallyCentered))
    {
        offset.x = (parentSize.x - childSize.x) / 2.f;
    }
    else if (isBitSet(alignment, Alignment::Right))
    {
        offset.x = parentSize.x - childSize.x;
    }

    if (isBitSet(alignment, Alignment::Top))
    {
        offset.y = 0;
    }
    else if (isBitSet(alignment, Alignment::VerticallyCentered))
    {
        offset.y = (parentSize.y - childSize.y) / 2.f;
    }
    else if (isBitSet(alignment, Alignment::Bottom))
    {
        offset.y = parentSize.y - childSize.y;
    }

    return offset;
}

sf::Vector2f boundsToSize(const sf::FloatRect& bounds)
{
    return sf::Vector2f{bounds.width, bounds.height};
}

}  // namespace gui
