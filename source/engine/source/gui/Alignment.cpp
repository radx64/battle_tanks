#include "engine/gui/Alignment.hpp"

namespace engine::gui
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

engine::Vec2f calculateAlignmentOffset(const engine::Vec2f& parentSize, const engine::Vec2f& childSize, const Alignment& alignment)
{
    engine::Vec2f offset {0.0f, 0.0f};

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

engine::Vec2f boundsToSize(const engine::FloatRect& bounds)
{
    return engine::Vec2f{bounds.width, bounds.height};
}

}  // namespace engine::gui
