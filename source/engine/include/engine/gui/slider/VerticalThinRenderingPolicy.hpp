#pragma once

#include <string>

#include "engine/Vector2.hpp"

namespace engine::gui::slider
{

class VerticalThinRenderingPolicy
{
public:
    static engine::Vector2f getTrackSize(const engine::Vector2f& sliderSize);
    static engine::Vector2f getTrackPosition(const engine::Vector2f& sliderPosition, const engine::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static engine::Vector2f getThumbSize(const engine::Vector2f& sliderSize, const float ratio);
    static engine::Vector2f getThumbPosition(const engine::Vector2f& sliderPosition, const engine::Vector2f& sliderSize, 
        const engine::Vector2f& trackSize, const engine::Vector2f& thumbSize, const float normalizedValue);
};

}  // namespace engine::gui::slider
