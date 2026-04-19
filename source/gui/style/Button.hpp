#pragma once

#include <string_view>

#include <SFML/Graphics/Color.hpp>
#include <SFML/Graphics/Texture.hpp>

#include "gui/style/Stateful.hpp"
#include "gui/style/Text.hpp"

namespace gui::style
{

struct Face
{
    Stateful<sf::Color> color;
    Stateful<std::string_view> texture;
};

struct Button
{
    Face face;
    Text text;
};

// TODO:
// Do similar for both Text and Icon Buttons,
// They should have applyStyle methods to configure how they look based on the style, and then we can have a ButtonStyle that has both Text and Icon Button styles, and then we can have a applyStyle method that applies the style to both Text and Icon Buttons.

// Same for windows

}  // gui::style
