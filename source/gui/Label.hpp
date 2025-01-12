#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
#include "gui/Component.hpp"
#include "gui/Text.hpp"

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
    gui::Alignment alignment_;
    gui::Text text_;
};

}  // namespace gui
