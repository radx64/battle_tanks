#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Widget.hpp"

namespace gui::layout
{

class Base : public Widget
{
public:
    void onRender(sf::RenderTexture& renderWindow) override;
};

}  // namespace gui::layout
