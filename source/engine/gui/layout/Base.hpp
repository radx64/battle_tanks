#pragma once

#include <SFML/Graphics.hpp>

#include "engine/gui/Widget.hpp"

namespace engine::gui::layout
{

class Base : public Widget
{
public:
    void onRender(sf::RenderTexture&render_window) override;
};

}  // namespace engine::gui::layout
