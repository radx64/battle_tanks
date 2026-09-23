#include "engine/gui/Checkbox.hpp"

#include "engine/gui/Alignment.hpp"
#include "engine/gui/TextureLibrary.hpp"

namespace engine::gui
{


Checkbox::Checkbox(const bool isChecked,
    const engine::gfx::Texture& normalTexture,
    const engine::gfx::Texture& hoverTexture,
    const engine::gfx::Texture& focusTexture,
    const engine::gfx::Texture& checkedTexture)
: isChecked_{isChecked}
, state_{State::Normal}
, background_(engine::gfx::Sprite())
, tick_(engine::gfx::Sprite())
, hoverTexture_(hoverTexture)
, focusTexture_(focusTexture)
, normalTexture_(normalTexture)
, checkedTexture_(checkedTexture)
{
    enableFocus();
    background_.setTexture(normalTexture_);
    tick_.setTexture(checkedTexture_);
}

Checkbox::Checkbox(const bool isChecked)
: Checkbox(isChecked
, TextureLibrary::instance().get("checkbox_normal")
, TextureLibrary::instance().get("checkbox_hover")
, TextureLibrary::instance().get("checkbox_focus")
, TextureLibrary::instance().get("checkbox_checked"))
{}

std::unique_ptr<Checkbox> Checkbox::create(const bool isChecked)
{
    return std::unique_ptr<Checkbox>{new Checkbox{isChecked}};
}

void Checkbox::onRender(engine::gfx::RenderTarget& renderTarget)
{
    renderTarget.draw(background_);
    if (isChecked_) renderTarget.draw(tick_);
}

void Checkbox::onStateChange(std::function<void(bool)> onClickCallback)
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
    if (onClick_)
    {
        onClick_(isChecked());
    }
}

bool Checkbox::isChecked() const
{
    return isChecked_;
}

EventStatus Checkbox::on(const engine::events::KeyReleased& keyboardKeyReleasedEvent)
{    
    if (not isFocused()) return EventStatus::NotConsumed;

    auto key = keyboardKeyReleasedEvent.getKeyCode();

    if (key == engine::core::KeyCode::Space || key == engine::core::KeyCode::Enter)
    {
        setState(!isChecked());
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;

}

EventStatus Checkbox::on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (mouseButtonReleasedEvent.getButton() != engine::core::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    setState(!isChecked());
    return EventStatus::Consumed;
}

EventStatus Checkbox::on(const engine::gui::events::MouseEntered&)
{
    state_ = State::Hover;
    updateTexture();
    return EventStatus::Consumed;
}

EventStatus Checkbox::on(const engine::gui::events::MouseLeft&)
{
    state_ = State::Normal;
    updateTexture();
    return EventStatus::Consumed;
}

void Checkbox::recalculateSpritesPositions()
{
    auto position = getGlobalPosition() + (getSize()/2.f) - boundsToSize(background_.getLocalBounds()) / 2.f;
    background_.setPosition(position);
    tick_.setPosition(position);
}

void Checkbox::onPositionChange()
{
    recalculateSpritesPositions();
}

void Checkbox::onSizeChange()
{
    recalculateSpritesPositions();
}

EventStatus Checkbox::on(const engine::gui::events::FocusLost&)
{
    updateTexture();
    return EventStatus::Consumed;
}

EventStatus Checkbox::on(const engine::gui::events::FocusGained&)
{
    updateTexture();
    return EventStatus::Consumed;
}


}  // namespace engine::gui
