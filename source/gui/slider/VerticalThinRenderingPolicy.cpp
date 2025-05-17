#include "gui/slider/VerticalThinRenderingPolicy.hpp"

#include "engine/Unused.hpp"
#include "gui/slider/Constants.hpp"

namespace gui::slider
{

sf::Vector2f VerticalThinRenderingPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{THIN_TRACK_THICKNESS, silderSize.y - BOUNDS_THICKNESS * 2.f};
}

sf::Vector2f VerticalThinRenderingPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    auto trackPositionOffset = sf::Vector2f{sliderSize.x / 2.f - THIN_TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    return sliderPosition + trackPositionOffset;
}

sf::Vector2f VerticalThinRenderingPolicy::getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue)
{
    UNUSED(thumbSize);
    auto thumbXPositionOffset = sliderSize.x / 2.f;
    auto thumbYPositionOffset = (1.f - normalizedValue) * trackSize.y + BOUNDS_THICKNESS;

    return sliderPosition + sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
}

std::string VerticalThinRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

sf::Vector2f VerticalThinRenderingPolicy::getThumbSize(const sf::Vector2f&)
{
    return sf::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS};
}


}  // namespace gui::slider
