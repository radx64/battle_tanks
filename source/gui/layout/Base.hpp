#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::layout
{

class Base : public Component
{
public:
    void onRender(sf::RenderTexture& renderWindow) override;
};

}  // namespace gui::layout
