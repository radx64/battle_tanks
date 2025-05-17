#include "gui/slider/VerticalThickRenderingPolicy.hpp"

#include "engine/Unused.hpp"
#include "gui/slider/Constants.hpp"

namespace gui::slider
{

sf::Vector2f VerticalThickRenderingPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x, silderSize.y};
}

std::string VerticalThickRenderingPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

sf::Vector2f VerticalThickRenderingPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    UNUSED(sliderSize);
    return sliderPosition;
}   

sf::Vector2f VerticalThickRenderingPolicy::getThumbSize(const sf::Vector2f& sliderSize)
{
    return sf::Vector2f{sliderSize.x, THUMB_THICKNESS};
}

sf::Vector2f VerticalThickRenderingPolicy::getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue)
{
    auto thumbXPositionOffset = sliderSize.x / 2.f;
    auto thumbYPositionOffset = (1.f - normalizedValue) * (trackSize.y - thumbSize.y) + thumbSize.y / 2.f;

    return sliderPosition + sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
}


}  // namespace gui::slider
