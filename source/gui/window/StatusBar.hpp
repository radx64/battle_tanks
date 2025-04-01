#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::window
{

//TODO: add statusbar inset texture

class StatusBar : public Component
{
public:
    StatusBar();

    void enable();
    void disable();
    bool isInsideResizeGadget(const sf::Vector2f point);

protected:
    void onRender(sf::RenderTexture& renderWindow) override;
    void onSizeChange() override;
    void onPositionChange() override;

    sf::Sprite resize_gadget_;
    const sf::Texture& resize_gadget_texture_;
};

}  // namespace gui::window
