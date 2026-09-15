#include "engine/gui/slider/VerticalThickRenderingPolicy.hpp"

#include "Unused.hpp"
#include "engine/gui/slider/Constants.hpp"

namespace engine::gui::slider
{

engine::Vec2f VerticalThickRenderingPolicy::getTrackSize(const engine::Vec2f& silderSize)
{
    return engine::Vec2f{silderSize.x, silderSize.y};
}

std::string VerticalThickRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

engine::Vec2f VerticalThickRenderingPolicy::getTrackPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize)
{
    UNUSED(sliderSize);
    return sliderPosition;
}   

engine::Vec2f VerticalThickRenderingPolicy::getThumbSize(const engine::Vec2f& sliderSize, const float ratio)
{
    return engine::Vec2f{sliderSize.x, sliderSize.y * ratio};
}

engine::Vec2f VerticalThickRenderingPolicy::getThumbPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize, 
    const engine::Vec2f& trackSize, const engine::Vec2f& thumbSize, const float normalizedValue)
{
    auto thumbXPositionOffset = sliderSize.x / 2.f;

    auto thumbYPositionOffset =  thumbSize.y / 2.f;

    if ((trackSize.y - thumbSize.y) > 0.f)
    {
        thumbYPositionOffset = (1.f - normalizedValue) * (trackSize.y - thumbSize.y) + thumbSize.y / 2.f;
    }

    return sliderPosition + engine::Vec2f{thumbXPositionOffset, thumbYPositionOffset};
}

}  // namespace engine::gui::slider
