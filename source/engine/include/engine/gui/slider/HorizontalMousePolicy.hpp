#pragma once


#include <engine/Vector2.hpp>

namespace engine::gui::slider
{

class HorizontalMousePolicy
{
public:
    static float translatePositionToThumbValue(const engine::Vec2f& mouse_position, const engine::Vec2f& trackPosition,
        const engine::Vec2f& trackSize, const engine::Vec2f& thumbSize, const float min, const float max, const float step);
};

}  // namespace engine::gui::slider
