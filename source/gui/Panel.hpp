#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui
{

class Panel : public Component
{
public:
    Panel();

protected:
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderWindow) override;
    void onFocus() override;
    void onFocusLost() override;

    sf::RectangleShape background_;
};

}  // namespace gui