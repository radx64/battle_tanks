#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/StyleSheet.hpp"

namespace gui { class IconButton; }
namespace gui { class Label; }

namespace gui::window
{

class Header : public Component
{
public:
    Header();

    void closeButtonAction(std::function<void()> closeButtonAction);
    void maximizeRestoreButtonAction(std::function<void()> maximizeRestoreButtonAction);
    void setTitle(const std::string_view& text);
    void enable();
    void disable();
    void setMaximizeRestoreButtonState(const bool& state);

protected:
    void onRender(sf::RenderTexture& renderWindow) override;
    void onSizeChange() override;
    void onPositionChange() override;

    gui::Label* titleTextPtr_;
    sf::RectangleShape headerShape_;
    gui::IconButton* closeButtonPtr_;
    gui::IconButton* maximizeRestoreButtonPtr_;
    std::function<void()> closeButtonAction_;
    std::function<void()> maximizeRestoreButtonAction_;
};

}  // namespace gui::window
