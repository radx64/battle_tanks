#pragma once

#include "engine/gfx/Font.hpp"

namespace engine::gui 
{

float getFontHeight(const engine::gfx::Font& font, unsigned int characterSize);

float getFontTop(const engine::gfx::Font& font, unsigned int characterSize);

float getFontLineSpacing(const engine::gfx::Font& font, unsigned int characterSize);

}  // namesapace gui
