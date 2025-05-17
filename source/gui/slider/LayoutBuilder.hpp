#pragma once

#include <SFML/Graphics.hpp>

#include "gui/FramedSprite.hpp"

namespace gui::slider
{

gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const gui::FramedSprite::LayoutConfig::UVs& uvs);
gui::FramedSprite::LayoutConfig::UVs buildUVsForTrackTexture();
gui::FramedSprite::LayoutConfig buildLayoutConfigForTrackTexture();
gui::FramedSprite::LayoutConfig::UVs buildUVsForThumbTexture();
gui::FramedSprite::LayoutConfig buildLayoutConfigForThumbTexture();

}  // namespace gui::slider 
