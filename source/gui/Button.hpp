#ifndef GUI_BUTTON_HPP_
#define GUI_BUTTON_HPP_

#include <functional>
#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class Button : public Component
{
public:
    Button(const std::string_view& text);
    ~Button();

    void setSize(const sf::Vector2f& size);
    void setText(const sf::String& text);
    void onRender(sf::RenderWindow& renderWindow) override;
    float getWidth() override;
    float getHeight() override;
    void onClick(std::function<void()> onClickCallback);
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked);

protected:
    sf::RectangleShape shape_;
    gui::Label* text_;
    bool wasButtonClicked_;
    std::function<void()> on_click_;
};

}  // namespace gui

#endif  // GUI_BUTTON_HPP_