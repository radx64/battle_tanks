#pragma once

#include <string>

#include "engine/Vector2.hpp"

namespace engine::gui::slider
{

class VerticalThickRenderingPolicy
{
public:
    static engine::Vec2f getTrackSize(const engine::Vec2f& silderSize);
    static engine::Vec2f getTrackPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize);
    static std::string getTrackTextureName();

    static engine::Vec2f getThumbSize(const engine::Vec2f& sliderSize, const float ratio);
    static engine::Vec2f getThumbPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize, 
        const engine::Vec2f& trackSize, const engine::Vec2f& thumbSize, const float normalizedValue);
};

}  // namespace engine::gui::slider
