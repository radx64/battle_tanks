#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Widget.hpp"

namespace gui::window
{

class Panel : public Widget
{
public:
    Panel();

    void enable();
    void disable();

    void addChild(std::unique_ptr<Widget> child) override;

protected:
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderWindow) override;
};

}  // namespace gui::window
