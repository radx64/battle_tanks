#ifndef GUI_BUTTON_HPP_
#define GUI_BUTTON_HPP_

#include <functional>
#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{

class Button : public Component
{
public:
    Button(Component* parent, const std::string_view& text)
    : Component(parent)
    , wasButtonClicked_(false)
    {
        auto style = BasicStyleSheetFactory::create();    
        shape_.setFillColor(style.getWindowColor());
        shape_.setOutlineColor(style.getOutlineColor());
        shape_.setOutlineThickness(style.getOutlineThickness());
        text_.setFont(style.getFont());
        text_.setCharacterSize(style.getFontSize());
        text_.setFillColor(style.getFontColor());
        text_.setOutlineColor(style.getOutlineColor());
        text_.setOutlineThickness(style.getOutlineThickness());
        text_.setString(text.data()); 
    }

    Button() : Button(nullptr, "")
    {}

    void set_size(const sf::Vector2f& size)
    {
        shape_.setSize(size);
        update_global_position();
    }

    void set_text(const sf::String& text)
    {
        text_.setString(text);
    }

    void on_render(sf::RenderWindow& renderWindow) override
    {
        shape_.setPosition(global_position_);
        renderWindow.draw(shape_);
        
        text_.setPosition(global_position_);
        renderWindow.draw(text_);
    }

    float get_width() override
    {
        return shape_.getGlobalBounds().width;
    }

    void on_click(std::function<void()> onClickCallback)
    {
        on_click_ = onClickCallback;
    }

    bool on_mouse_update(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        if (shape_.getGlobalBounds().contains(mousePosition))
        {
            shape_.setFillColor(sf::Color(255,0,0,255));
            if (isLeftClicked && !wasButtonClicked_)
            {
                wasButtonClicked_ = true;
                if (on_click_) on_click_();
            }
            if (!isLeftClicked)
            {
                wasButtonClicked_ = false;
            }
            return true;
        }
        else
        {
            shape_.setFillColor(sf::Color(0,255,0,255));
            return false;
        }
    }

protected:
    sf::RectangleShape shape_;
    sf::Text text_;
    bool wasButtonClicked_;
    std::function<void()> on_click_;
};

}  // namespace gui

#endif  // GUI_BUTTON_HPP_