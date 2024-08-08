#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
#include "gui/Component.hpp"

namespace gui
{

class Label : public Component
{
public:
    Label(const std::string_view& text);

    void setText(const std::string_view& text);
    void setAlignment(const gui::Alignment& alignment);

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;
    void recalculatePositionAndSize();
    sf::Text text_;
    gui::Alignment alignment_;
};

}  // namespace gui

