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
    bool onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked) override;
    void close();
    bool isDead() const;
    void focus();
    void defocus();
    bool isFocused() const;

protected:
    enum class State
    {
        Idle,
        Dragging,
        Resizing
    };

    bool isInsideTopBar(const sf::Vector2f point);
    bool isInsideResizeThingy(const sf::Vector2f point);
    bool isState(const Window::State& state);
    void updateResizeThingy();

    sf::RectangleShape background_;
    sf::RectangleShape top_bar_;
    sf::RectangleShape resize_thingy_;
    gui::Label* title_text_handle_;
    bool killed_;
    bool focused_;
    State state_;
    sf::Vector2f dragging_offset_;
    StyleSheet style_;
};

}  // namespace gui

#endif  // GUI_WINDOW_HPP_