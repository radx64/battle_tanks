#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{
Button::Button(const std::string_view& text)
: wasButtonClicked_(false)
{
    auto style = BasicStyleSheetFactory::create();    
    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness()); 
    auto text_ptr = std::make_unique <gui::Label>(text);
    text_ = text_ptr.get();
    this->addChild(std::move(text_ptr));
}

Button::~Button() = default;

void Button::setSize(const sf::Vector2f& size)
{
    background_.setSize(size);
    Component::setSize(size);
    updateGlobalPosition();
}

void Button::setText(const sf::String& text)
{
    text_->setText(text);
}

void Button::onRender(sf::RenderWindow& renderWindow)
{
    background_.setPosition(Component::getGlobalPosition());
    renderWindow.draw(background_);
    
    //FIXME: this should not be called here as position should be recalculated on changes not every renderframe
    text_->setPosition(getSize() / 2.f, gui::Alignment::CENTERED);
}

void Button::onClick(std::function<void()> onClickCallback)
{
    on_click_ = onClickCallback;
}

bool Button::onMouseUpdate(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    if (background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(255,0,0,255));

        if(isLeftClicked)
        {
            background_.setFillColor(sf::Color(0,0,255,255));     
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
        background_.setFillColor(sf::Color(0,255,0,255));
        return false;
    }
}

}  // namespace gui