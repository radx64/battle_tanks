#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Window : public Component
{
public:
    Window();

    void setSize(const sf::Vector2f& size);
    sf::Vector2f getSize();
    bool isInside(const sf::Vector2f point);
    void onRender(sf::RenderWindow& renderWindow) override;
    float getWidth() override;
    float getHeight() override;
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked);
    void close();
    bool isDead();
    void focus();
    void defocus();
    bool hasFocus();

protected:
    sf::RectangleShape shape_;
    bool killed_;
    bool focused_;
    StyleSheet style_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_