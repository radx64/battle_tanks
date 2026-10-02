#pragma once


#include "engine/gui/FramedSprite.hpp"

namespace engine::gui::slider
{

engine::gui::FramedSprite::LayoutConfig buildLayoutConfig(const engine::Vec2f& cornerSizes, const engine::gui::FramedSprite::LayoutConfig::UVs& uvs);
engine::gui::FramedSprite::LayoutConfig::UVs buildUVsForTrackTexture();
engine::gui::FramedSprite::LayoutConfig buildLayoutConfigForTrackTexture();
engine::gui::FramedSprite::LayoutConfig::UVs buildUVsForThumbTexture();
engine::gui::FramedSprite::LayoutConfig buildLayoutConfigForThumbTexture();

}  // namespace engine::gui::slider 
