#include "gui/Button.hpp"

#include "gui/Label.hpp"
#include "gui/StyleSheet.hpp"

namespace gui
{
Button::Button(const std::string_view& text)
: isButtonHoldDown_(false)
{
    enableFocus();
    auto style = BasicStyleSheetFactory::instance();
    background_.setFillColor(style.getWindowColor());
    background_.setOutlineColor(style.getOutlineColor());
    background_.setOutlineThickness(style.getOutlineThickness());
    background_.setPosition(getGlobalPosition());
    background_.setSize(Component::getSize());
    auto textPtr = std::make_unique <gui::Label>(text);
    text_ = textPtr.get();
    text_->setAlignment(gui::Alignment::HorizontallyCentered | gui::Alignment::VerticallyCentered);
    addChild(std::move(textPtr));
}

void Button::onSizeChange()
{
    auto buttonSize = getSize();
    background_.setSize(buttonSize);
    text_->setSize(buttonSize);
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
    onClick_ = onClickCallback;
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
    if (not isVisible_) return gui::EventStatus::NotConsumed;

    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    bool isLeftClicked = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftClicked and background_.getGlobalBounds().contains(mousePosition))
    {
        background_.setFillColor(sf::Color(0,255,0,255));
        if (not isButtonHoldDown_)
        {
            isButtonHoldDown_ = true;
            if (onClick_) onClick_();
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
