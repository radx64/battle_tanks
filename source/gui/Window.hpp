#ifndef GUI_WINDOW_HPP_
#define GUI_WINDOW_HPP_

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/StyleSheet.hpp"

namespace gui { class Label; }
namespace gui
{

class Window : public Component
{
public:
    Window();

    void setSize(const sf::Vector2f& size) override;
    void setPosition(const sf::Vector2f& position, const Alignment alignment) override;
    void setTitle(const std::string_view& text);
    bool isInside(const sf::Vector2f point);
    void onRender(sf::RenderWindow& renderWindow) override;
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked);
    void close();
    bool isDead();
    void focus();
    void defocus();
    bool hasFocus();

protected:
    sf::RectangleShape background_;
    sf::RectangleShape top_bar_;
    gui::Label* title_text_handle_;
    bool killed_;
    bool focused_;
    StyleSheet style_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_