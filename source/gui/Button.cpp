#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{
Button::Button(const std::string_view& text)
: wasButtonClicked_(false)
{
    auto style = BasicStyleSheetFactory::create();    
    shape_.setFillColor(style.getWindowColor());
    shape_.setOutlineColor(style.getOutlineColor());
    shape_.setOutlineThickness(style.getOutlineThickness()); 
    auto text_ptr = std::make_unique <gui::Label>(text);
    text_ = text_ptr.get();
    this->addChild(std::move(text_ptr));
}

Button::~Button() = default;

void Button::setSize(const sf::Vector2f& size)
{
    shape_.setSize(size);
    updateGlobalPosition();
}

void Button::setText(const sf::String& text)
{
    text_->setText(text);
}

void Button::onRender(sf::RenderWindow& renderWindow)
{
    shape_.setPosition(global_position_);
    renderWindow.draw(shape_);
    
    text_->setPosition(sf::Vector2f(getWidth()/2.f, getHeight()/2.f), gui::Alignment::CENTERED);
}

float Button::getWidth()
{
    return shape_.getGlobalBounds().width;
}

float Button::getHeight()
{
    return shape_.getGlobalBounds().height;
}

void Button::onClick(std::function<void()> onClickCallback)
{
    on_click_ = onClickCallback;
}

bool Button::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    if (shape_.getGlobalBounds().contains(mousePosition))
    {
        shape_.setFillColor(sf::Color(255,0,0,255));

        if(isLeftClicked)
        {
            shape_.setFillColor(sf::Color(0,0,255,255));     
        }

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

}  // namespace gui