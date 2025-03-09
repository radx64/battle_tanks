#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::window
{

class Panel : public Component
{
public:
    Panel();

    void enable();
    void disable();

    void addChild(std::unique_ptr<Component> child) override;

protected:
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderWindow) override;

    sf::RectangleShape backgroundShape_;
};

}  // namespace gui::window
