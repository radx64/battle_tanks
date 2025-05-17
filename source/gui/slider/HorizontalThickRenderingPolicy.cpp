#include "gui/slider/HorizontalThickRenderingPolicy.hpp"

#include "engine/Unused.hpp"

#include "gui/slider/Constants.hpp"

namespace gui::slider
{

sf::Vector2f HorizontalThickRenderingPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x, silderSize.y};
}

std::string HorizontalThickRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

sf::Vector2f HorizontalThickRenderingPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    UNUSED(sliderSize);
    return sliderPosition;
}

sf::Vector2f HorizontalThickRenderingPolicy::getThumbSize(const sf::Vector2f& sliderSize)
{
    return sf::Vector2f{THUMB_THICKNESS, sliderSize.y};
}

sf::Vector2f HorizontalThickRenderingPolicy::getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue)
{
    auto thumbXPositionOffset = normalizedValue * (trackSize.x - thumbSize.x) + thumbSize.x / 2.f;
    auto thumbYPositionOffset = sliderSize.y / 2.f;

    return sliderPosition + sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
}

}  // namespace gui::slider
