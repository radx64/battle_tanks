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
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());
    auto text_ptr = std::make_unique <gui::Label>(text);
    text_ = text_ptr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(text_ptr));
}

void Button::onSizeChange()
{
    auto button_size = getSize();
    background_.setSize(button_size);
    text_->setSize(button_size);
}

void Button::onPositionChange()
{
    background_.setPosition(Component::getGlobalPosition());
}

void Button::setText(const std::string_view& text)
{
    text_->setText(text);
    text_->setSize(getSize());
}

void Button::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

void Button::onClick(std::function<void()> onClickCallback)
{
    on_click_ = onClickCallback;
}

EventStatus Button::on(const event::MouseEntered&)
{
    background_.setFillColor(sf::Color(255,0,0,255));
    return gui::EventStatus::Consumed;
}
EventStatus Button::on(const event::MouseLeft&)
{
    background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
    return gui::EventStatus::Consumed;
}

EventStatus Button::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not is_visible_) return gui::EventStatus::NotConsumed;
    
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    bool isLeftClicked = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked and background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(0,255,0,255));
        if (not isButtonHoldDown_) 
        {
            isButtonHoldDown_ = true;
            if (on_click_) on_click_();  
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
        background_.setFillColor(BasicStyleSheetFactory::instance().getWindowColor());
        return gui::EventStatus::Consumed;
    }
    return gui::EventStatus::NotConsumed;
}

}  // namespace gui