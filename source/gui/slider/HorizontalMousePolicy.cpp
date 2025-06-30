#include "gui/slider/HorizontalMousePolicy.hpp"

#include <cmath>

namespace gui::slider
{

float HorizontalMousePolicy::translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& trackPosition,
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step)
{
    auto availableTruckRun = trackSize.x - thumbSize.x;

    auto thumbXPositionOffset = mousePosition.x  - trackPosition.x - thumbSize.x / 2.f;

    // Calculate screen step size
    float stepSize = availableTruckRun  / (max - min) * step;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbXPosition = std::round(thumbXPositionOffset / stepSize) * stepSize;
    // Normalize to 0.f to 1.f range
    float normalizedThumbXPosition = stepAlignedThumbXPosition / (availableTruckRun);
    // Map to set range
    float rangeMappedThumbXPosition = (max - min) * normalizedThumbXPosition + min;

    return std::clamp(rangeMappedThumbXPosition, min, max);
}

}  // namespace gui::slider
