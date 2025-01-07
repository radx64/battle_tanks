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

sf::Vector2f calculateAlignmentOffset(const sf::Vector2f& size, const sf::FloatRect& bounds, const Alignment& alignment)
{
    sf::Vector2f offset {0.0f, 0.0f};

    if (isBitSet(alignment, Alignment::Left))
    {
        offset.x = 0;
    }
    else if (isBitSet(alignment, Alignment::HorizontallyCentered))
    {
        offset.x = size.x / 2 - bounds.width / 2;
    }
    else if (isBitSet(alignment, Alignment::Right))
    {
        offset.x = size.x - bounds.width;
    }

    if (isBitSet(alignment, Alignment::Top))
    {
        offset.y = 0;
    }
    else if (isBitSet(alignment, Alignment::VerticallyCentered))
    {
        offset.y = size.y / 2 - bounds.height / 2;
    }
    else if (isBitSet(alignment, Alignment::Bottom))
    {
        offset.y = size.y - bounds.height;
    }

    return offset;
}

}  // namespace gui
