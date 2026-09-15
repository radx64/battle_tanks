#include "engine/gui/slider/HorizontalThickRenderingPolicy.hpp"

#include "Unused.hpp"

#include "engine/gui/slider/Constants.hpp"

namespace engine::gui::slider
{

engine::Vec2f HorizontalThickRenderingPolicy::getTrackSize(const engine::Vec2f& sliderSize)
{
    return engine::Vec2f{sliderSize.x, sliderSize.y};
}

std::string HorizontalThickRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

engine::Vec2f HorizontalThickRenderingPolicy::getTrackPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize)
{
    UNUSED(sliderSize);
    return sliderPosition;
}

engine::Vec2f HorizontalThickRenderingPolicy::getThumbSize(const engine::Vec2f& sliderSize, const float ratio)
{
    return engine::Vec2f{sliderSize.x * ratio, sliderSize.y};
}

engine::Vec2f HorizontalThickRenderingPolicy::getThumbPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize, 
    const engine::Vec2f& trackSize, const engine::Vec2f& thumbSize, const float normalizedValue)
{
    auto thumbXPositionOffset = thumbSize.x / 2.f;

    if ((trackSize.x - thumbSize.x) > 0.f)
    {
        thumbXPositionOffset = normalizedValue * (trackSize.x - thumbSize.x) + thumbSize.x / 2.f;

    }

    auto thumbYPositionOffset = sliderSize.y / 2.f;

    return sliderPosition + engine::Vec2f{thumbXPositionOffset, thumbYPositionOffset};
}

}  // namespace engine::gui::slider
