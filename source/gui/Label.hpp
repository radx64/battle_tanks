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

    void onRender(sf::RenderTexture& renderTexture) override;
    void setText(const sf::String& text);
    void setAlignment(const gui::Alignment& alignment);


protected:
    void onSizeChange() override;
    void onPositionChange() override;
    void recalculatePositionAndSize();
    sf::Text text_;
    gui::Alignment alignment_;
};

}  // namespace gui

