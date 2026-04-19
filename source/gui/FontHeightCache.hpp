#pragma once

namespace sf{ class Font; }

namespace gui 
{

float getFontHeight(const sf::Font& font, unsigned int characterSize);

float getFontTop(const sf::Font& font, unsigned int characterSize);

float getFontLineSpacing(const sf::Font& font, unsigned int characterSize);

}  // namesapace gui
