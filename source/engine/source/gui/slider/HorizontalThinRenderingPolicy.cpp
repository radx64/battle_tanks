#include "engine/gui/slider/HorizontalThinRenderingPolicy.hpp"

#include "Unused.hpp"

#include "engine/gui/slider/Constants.hpp"


namespace engine::gui::slider
{

engine::Vec2f HorizontalThinRenderingPolicy::getTrackSize(const engine::Vec2f& silderSize)
{
    return engine::Vec2f{silderSize.x - BOUNDS_THICKNESS * 2.f, THIN_TRACK_THICKNESS};
}

engine::Vec2f HorizontalThinRenderingPolicy::getTrackPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize)
{
    auto trackPositionOffset = engine::Vec2f{BOUNDS_THICKNESS, sliderSize.y / 2.f - THIN_TRACK_THICKNESS / 2.f};
    return sliderPosition + trackPositionOffset;
}

std::string HorizontalThinRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

engine::Vec2f HorizontalThinRenderingPolicy::getThumbSize(const engine::Vec2f&, const float)
{
    return engine::Vec2f{THUMB_THICKNESS, THUMB_THICKNESS};
}

engine::Vec2f HorizontalThinRenderingPolicy::getThumbPosition(const engine::Vec2f& sliderPosition, const engine::Vec2f& sliderSize, 
    const engine::Vec2f& trackSize, const engine::Vec2f& thumbSize, const float normalizedValue)
{
    auto thumbXPositionOffset = normalizedValue * (trackSize.x - thumbSize.x) + thumbSize.x;
    auto thumbYPositionOffset = sliderSize.y / 2.f;

    return sliderPosition + engine::Vec2f{thumbXPositionOffset, thumbYPositionOffset};
}

}  // namespace engine::gui::slider
