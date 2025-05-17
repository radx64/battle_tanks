#pragma once

#include <string>

#include <SFML/Graphics.hpp>

namespace gui::slider
{

class VerticalThinRenderingPolicy
{
public:
    static sf::Vector2f getTrackSize(const sf::Vector2f& silderSize);
    static sf::Vector2f getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static sf::Vector2f getThumbSize(const sf::Vector2f& sliderSize);
    static sf::Vector2f getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue);
};

}  // namespace gui::slider
