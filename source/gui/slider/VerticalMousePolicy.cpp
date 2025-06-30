#include "gui/slider/VerticalMousePolicy.hpp"

#include <cmath>

namespace gui::slider
{

float VerticalMousePolicy::translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& trackPosition,
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step)
{
    auto availableTruckRun = trackSize.y - thumbSize.y;

    auto thumbYPositionOffset = trackPosition.y + trackSize.y - mousePosition.y - thumbSize.y / 2.f;

    // Calculate screen step size
    float stepSize = availableTruckRun  / (max - min) * step;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbYPosition = std::round(thumbYPositionOffset / stepSize) * stepSize;
    // Normalize to 0.f to 1.f range
    float normalizedThumbYPosition = stepAlignedThumbYPosition / (availableTruckRun);
    // Map to set range
    float rangeMappedThumbYPosition = (max - min) * normalizedThumbYPosition + min;

    return std::clamp(rangeMappedThumbYPosition, min, max);
}
    
}  // namespace gui::slider
