#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/StyleSheet.hpp"

namespace gui { class Button; }
namespace gui { class Label; }

namespace gui::window
{

class Header : public Component
{
public:
    Header();

    void closeButtonAction(std::function<void()> closeButtonAction);
    void setTitle(const std::string_view& text);
    void enable();
    void disable();

protected:
    void onRender(sf::RenderTexture& renderWindow) override;
    void onSizeChange() override;
    void onPositionChange() override;

    gui::Label* title_text_handle_;
    sf::RectangleShape header_shape_;
    gui::Button* close_button_ptr_;
    std::function<void()> closeButtonAction_;
};

}  // namespace gui::window
