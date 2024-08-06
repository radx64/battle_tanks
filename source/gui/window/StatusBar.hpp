#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::window
{

class StatusBar : public Component
{
public:
    StatusBar();

    void activate();
    void deactivate();
    bool isInsideResizeGadget(const sf::Vector2f point);

protected:
    void onRender(sf::RenderTexture& renderWindow) override;
    void onSizeChange() override;
    void onPositionChange() override;

    sf::RectangleShape resize_gadget_;
    sf::RectangleShape background_;
};


}  // namespace gui::window