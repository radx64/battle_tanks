#include "gui/slider/HorizontalThinRenderingPolicy.hpp"

#include "engine/Unused.hpp"

#include "gui/slider/Constants.hpp"


namespace gui::slider
{

sf::Vector2f HorizontalThinRenderingPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x - BOUNDS_THICKNESS * 2.f, THIN_TRACK_THICKNESS};
}

sf::Vector2f HorizontalThinRenderingPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, sliderSize.y / 2.f - THIN_TRACK_THICKNESS / 2.f};
    return sliderPosition + trackPositionOffset;
}

std::string HorizontalThinRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

sf::Vector2f HorizontalThinRenderingPolicy::getThumbSize(const sf::Vector2f&)
{
    return sf::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS};
}

sf::Vector2f HorizontalThinRenderingPolicy::getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue)
{
    UNUSED(thumbSize);
    auto thumbXPositionOffset = normalizedValue * (trackSize.x - thumbSize.x) + thumbSize.x;
    auto thumbYPositionOffset = sliderSize.y / 2.f;

    return sliderPosition + sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
}


}  // namespace gui::slider
