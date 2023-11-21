#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{
Button::Button(const std::string_view& text)
: isButtonHoldDown_(false)
{
    auto style = BasicStyleSheetFactory::instance();    
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

EventStatus Button::on(const event::MouseMoved& mouseMovedEvent)
{
    if (isButtonHoldDown_) return gui::EventStatus::NotConsumed;

    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    if (background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(255,0,0,255)); 
    }
    else
    {
        background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    }

    // Don't need to consume this event as this only updates visuals
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::MouseButtonPressed& mousePressedEvent)
{
    auto mousePosition = sf::Vector2f{mousePressedEvent.position.x, mousePressedEvent.position.y};
    bool isLeftClicked = mousePressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked and background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(0,0,255,255));
        if (not isButtonHoldDown_ and on_click_) 
        {
            isButtonHoldDown_ = true; 
            on_click_();  
        }
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

EventStatus Button::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (not isButtonHoldDown_) return gui::EventStatus::NotConsumed;
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased)
    {
        isButtonHoldDown_ = false;
        background_.setFillColor(sf::Color(0,255,0,255));
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

}  // namespace gui