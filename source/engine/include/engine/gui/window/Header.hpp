#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "engine/gui/Widget.hpp"

namespace engine::gui { class IconButton; }
namespace engine::gui { class Label; }
namespace engine::gui::style { struct Window; }

namespace engine::gui::window
{

class Header : public Widget
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
    void onRender(sf::RenderTexture&render_window) override;
    void onSizeChange() override;
    void onPositionChange() override;
    EventStatus on(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;

    engine::gui::Label* titleTextPtr_;
    sf::RectangleShape headerShape_;
    engine::gui::IconButton* closeButtonPtr_;
    engine::gui::IconButton* maximizeRestoreButtonPtr_;
    std::function<void()> closeButtonAction_;
    std::function<void()> maximizeRestoreButtonAction_;

    const style::Window& style_;
};

}  // namespace engine::gui::window
