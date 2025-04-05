#include "gui/Checkbox.hpp"

#include "gui/Alignment.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui
{

Checkbox::Checkbox(bool isChecked)
    : isChecked_{isChecked}
    , state_{State::Normal}
    , background_(sf::Sprite())
    , tick_(sf::Sprite())
    , hoverTexture_(TextureLibrary::instance().get("checkbox_hover"))
    , focusTexture_(TextureLibrary::instance().get("checkbox_focus"))
    , normalTexture_(TextureLibrary::instance().get("checkbox_normal"))
    , checkedTexture_(TextureLibrary::instance().get("checkbox_checked"))
{
    enableFocus();
    background_.setTexture(normalTexture_);
    tick_.setTexture(checkedTexture_);
}

std::unique_ptr<Checkbox> Checkbox::create(const bool isChecked)
{
    return std::unique_ptr<Checkbox>{new Checkbox{isChecked}};
}

void Checkbox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
    if (isChecked_) renderTexture.draw(tick_);
}

void Checkbox::onClick(std::function<void(bool)> onClickCallback)
{
    onClick_ = onClickCallback;
}

void Checkbox::updateTexture()
{
    switch (state_)
    {
        case State::Normal:
            if (isFocused())
            {
                background_.setTexture(focusTexture_);
            }
            else
            {
                background_.setTexture(normalTexture_);
            }
            break;
        case State::Hover:
            background_.setTexture(hoverTexture_);
            break;
    } 
}

void Checkbox::setState(const bool checked)
{
    isChecked_ = checked;
    updateTexture();
}

bool Checkbox::isChecked() const
{
    return isChecked_;
}

EventStatus Checkbox::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Space || key == gui::event::Key::Enter)
    {
        isChecked_ = !isChecked_;
        updateTexture();
        
        if (onClick_) onClick_(isChecked());
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;

}

EventStatus Checkbox::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.button != gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }
    
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};

    if (not isInside(mousePosition))
    {
        return EventStatus::NotConsumed;
    }

    isChecked_ = !isChecked_;
    updateTexture();
    if (onClick_)
    {
        onClick_(isChecked_);
    }
    return EventStatus::Consumed;

    
    return EventStatus::NotConsumed;
}

EventStatus Checkbox::on(const event::MouseEntered&)
{
    state_ = State::Hover;
    updateTexture();
    return EventStatus::Consumed;
}

EventStatus Checkbox::on(const event::MouseLeft&)
{
    state_ = State::Normal;
    updateTexture();
    return EventStatus::Consumed;
}

void Checkbox::onPositionChange()
{
    auto position = getGlobalPosition() + (getSize()/2.f) - boundsToSize(background_.getLocalBounds()) / 2.f;
    background_.setPosition(position);
    tick_.setPosition(position);
}

EventStatus Checkbox::on(const event::FocusLost&)
{
    updateTexture();
    return EventStatus::Consumed;
}

EventStatus Checkbox::on(const event::FocusGained&)
{
    updateTexture();
    return EventStatus::Consumed;
}


}  // namespace gui
