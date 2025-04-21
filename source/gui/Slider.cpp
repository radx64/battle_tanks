#include "gui/Slider.hpp"

#include <cmath>

#include "engine/math/Math.hpp"


namespace gui
{

constexpr float TRACK_THICKNESS = 5.f;
constexpr float THUMB_THICKNESS = 20.f;
constexpr float BOUNDS_THICKNESS = 10.f;
constexpr float STEP = 0.05f;

std::unique_ptr<Slider> Slider::create()
{
    return std::unique_ptr<Slider>{new Slider{}};
}

Slider::Slider()
: value_{0.5f}
, onValueChange_{nullptr}
, state_{State::Idle}
{
    enableFocus();
    track_.setFillColor(sf::Color(100, 100, 100));
    track_.setSize(sf::Vector2f{TRACK_THICKNESS, TRACK_THICKNESS});
    thumb_.setFillColor(sf::Color(100, 100, 100));
    thumb_.setSize(sf::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS});
    thumb_.setOrigin(THUMB_THICKNESS / 2.f, THUMB_THICKNESS / 2.f);
}

void Slider::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(track_);
    renderTexture.draw(thumb_);
}

void Slider::onValueChange(std::function<void(float)> onValueChangeCallback)
{
    onValueChange_ = onValueChangeCallback;
}

void Slider::onSizeChange()
{
    track_.setSize(sf::Vector2f{getSize().x - BOUNDS_THICKNESS * 2.f, TRACK_THICKNESS});

}

void Slider::onPositionChange()
{
    updateTexture();
}


EventStatus Slider::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Left)
    {
        value_ -= STEP;
    }
    else if (key == gui::event::Key::Right)
    {
        value_ += STEP;
    }
    else
    {
        return EventStatus::NotConsumed;
    }

    value_ = engine::math::clamp(value_, 0.f, 1.f);
    updateTexture();
    if (onValueChange_) onValueChange_(value_);

    return EventStatus::Consumed;
}

EventStatus Slider::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    bool isLeftPressed = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftPressed and isInside(mousePosition))
    {
        processMovement(mousePosition);
        state_ = State::Dragging;
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

EventStatus Slider::on(const event::MouseMoved& mouseMovedEvent)
{
    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    if (state_ == State::Dragging)
    {
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

EventStatus Slider::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased and state_ == State::Dragging)
    {
        processMovement(mousePosition);
        state_ = State::Idle;
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

void Slider::processMovement(sf::Vector2f& mousePosition)
{
    value_ = translateMousePositionToThumbValue(mousePosition);
    focus();
    updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

void Slider::updateTexture()
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    track_.setPosition(getGlobalPosition() + trackPositionOffset);
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = value_ * (trackSize.x) + BOUNDS_THICKNESS;
    auto thumbYPositionOffset = getSize().y / 2.f;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

float Slider::translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    auto trackSize = track_.getSize();
    auto thumbXPositionOffset = mousePosition.x - getGlobalPosition().x - trackPositionOffset.x;

    // STEP below needs to be multiplied by track size as it varies depending on slider width
    float stepSize = STEP * trackSize.x;

    // Calculate the nearest step by rounding instead of truncating
    float alignedThumbXPosition = std::round(thumbXPositionOffset / stepSize) * stepSize;

    return engine::math::clamp(alignedThumbXPosition / trackSize.x, 0.f, 1.f);
}

}  // namespace gui
