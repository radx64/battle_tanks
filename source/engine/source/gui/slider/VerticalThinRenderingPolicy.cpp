#include "engine/gui/slider/VerticalThinRenderingPolicy.hpp"

#include "Unused.hpp"
#include "engine/gui/slider/Constants.hpp"

namespace engine::gui::slider
{

engine::Vector2f VerticalThinRenderingPolicy::getTrackSize(const engine::Vector2f& sliderSize)
{
    return engine::Vector2f{THIN_TRACK_THICKNESS, sliderSize.y - BOUNDS_THICKNESS * 2.f};
}

engine::Vector2f VerticalThinRenderingPolicy::getTrackPosition(const engine::Vector2f& sliderPosition, const engine::Vector2f& sliderSize)
{
    auto trackPositionOffset = engine::Vector2f{sliderSize.x / 2.f - THIN_TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    return sliderPosition + trackPositionOffset;
}

engine::Vector2f VerticalThinRenderingPolicy::getThumbPosition(const engine::Vector2f& sliderPosition, const engine::Vector2f& sliderSize, 
    const engine::Vector2f& trackSize, const engine::Vector2f& thumbSize, const float normalizedValue)
{
    UNUSED(thumbSize);
    auto thumbXPositionOffset = sliderSize.x / 2.f;
    auto thumbYPositionOffset = (1.f - normalizedValue) * (trackSize.y - thumbSize.y) + thumbSize.y;

    return sliderPosition + engine::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
}

std::string VerticalThinRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

engine::Vector2f VerticalThinRenderingPolicy::getThumbSize(const engine::Vector2f&, const float)
{
    return engine::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS};
}


}  // namespace engine::gui::slider
