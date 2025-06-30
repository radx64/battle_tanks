#pragma once

#include <functional>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/slider/LayoutBuilder.hpp"
#include "gui/TextureLibrary.hpp"

namespace gui::slider
{

/* TODO:
    - mouse wheel support
    - render steps marks (implement after focus frame rendering is done)
*/

template <typename MouseHandlingPolicy, typename RenderingPolicy>
class Base : public Component
{
public:
    void onValueChange(std::function<void(float)> onValueChangeCallback);

    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;
    void setStep(const float step);
    void increase();
    void decrease();

    void setThumbRatio(const float ratio);

protected:
    enum class State
    {
        Idle,
        Dragging,
        Hover   // TODO add disabled in the future
    };

    void onRender(sf::RenderTexture& renderTexture) override;
    Base();

    void onSizeChange() override;
    void onPositionChange() override;

    float normalizeValue();

    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

    void processMovement(sf::Vector2f& mousePosition);
    void updateTexture();

    gui::FramedSprite track_;
    gui::FramedSprite thumb_;

    const sf::Texture& hoverTexture_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    const sf::Texture& pressedTexture_;

    float value_;
    float step_;
    float min_;
    float max_;
    float thumbRatio_;
    
    std::function<void(float)> onValueChange_;
    State state_;
};

template <typename MouseHandlingPolicy, typename RenderingPolicy>
Base<MouseHandlingPolicy, RenderingPolicy>::Base()
: track_{buildLayoutConfigForTrackTexture()}
, thumb_{buildLayoutConfigForThumbTexture()}
, hoverTexture_{TextureLibrary::instance().get("button_hover")}
, focusTexture_{TextureLibrary::instance().get("button_focus")}
, normalTexture_{TextureLibrary::instance().get("button_normal")}
, pressedTexture_{TextureLibrary::instance().get("button_pressed")} // reusing button textures for now
, value_{0.0f}
, step_{0.01f}
, min_{0.0f}
, max_{1.0f}
, thumbRatio_{0.2f}
, onValueChange_{nullptr}
, state_{State::Idle}
{
    enableFocus();

    track_.setTexture(TextureLibrary::instance().get(RenderingPolicy::getTrackTextureName()));

    thumb_.setTexture(normalTexture_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(track_);
    renderTexture.draw(thumb_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::onValueChange(std::function<void(float)> onValueChangeCallback)
{
    onValueChange_ = onValueChangeCallback;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::setRange(const float min, const float max)
{
    min_ = min;
    max_ = max;
    setValue(getValue());
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::setValue(const float value)
{
    value_ = std::clamp(value, min_, max_);
    updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
float Base<MouseHandlingPolicy, RenderingPolicy>::getValue() const
{
    return value_;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::setStep(const float step)
{
    step_ = step;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::increase()
{
    setValue(getValue() + step_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::decrease()
{
    setValue(getValue() - step_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::onSizeChange()
{
    track_.setSize(RenderingPolicy::getTrackSize(getSize()));

    auto thumbSize = RenderingPolicy::getThumbSize(getSize(), thumbRatio_);
    thumb_.setSize(thumbSize);
    thumb_.setOrigin(thumbSize / 2.f);
    updateTexture();
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::onPositionChange()
{
    updateTexture();
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
float Base<MouseHandlingPolicy, RenderingPolicy>::normalizeValue()
{
    return (value_ - min_) / (max_ - min_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    if (not isFocused()) return EventStatus::NotConsumed;

    auto& key = keyboardKeyReleasedEvent.key;

    if (key == gui::event::Key::Left)
    {
        value_ -= step_;
    }
    else if (key == gui::event::Key::Right)
    {
        value_ += step_;
    }
    else
    {
        return EventStatus::NotConsumed;
    }

    value_ = std::clamp(value_, min_, max_);

    updateTexture();

    if (onValueChange_) onValueChange_(value_);

    return EventStatus::Consumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    bool isLeftPressed = mouseButtonPressedEvent.button == gui::event::MouseButton::Left;

    if (isLeftPressed and isInside(mousePosition))
    {
        state_ = State::Dragging;
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseMoved& mouseMovedEvent)
{
    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    if (state_ == State::Dragging)
    {
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    else
    {
        bool isMouseInside = isInside(mousePosition);
        if (isMouseInside and state_ != State::Hover)
        {
            state_ = State::Hover;
            updateTexture();
            return EventStatus::Consumed;
        }
        else if (not isMouseInside and state_ == State::Hover)
        {
            state_ = State::Idle;
            updateTexture();
            return EventStatus::Consumed;
        }
    }

    return EventStatus::NotConsumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonReleasedEvent.position.x, mouseButtonReleasedEvent.position.y};
    bool isLeftReleased = mouseButtonReleasedEvent.button == gui::event::MouseButton::Left;

    if (isLeftReleased and state_ == State::Dragging)
    {
        state_ = State::Idle;
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::processMovement(sf::Vector2f& mousePosition)
{
    value_ = MouseHandlingPolicy::translatePositionToThumbValue(mousePosition, track_.getPosition(), track_.getSize(), thumb_.getSize(), min_, max_, step_);

    focus();
    updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void Base<MouseHandlingPolicy, RenderingPolicy>::updateTexture()
{
    switch(state_)
    {
        case State::Idle:
            if(isFocused())
            {
                thumb_.setTexture(focusTexture_);
            }
            else
            {
                thumb_.setTexture(normalTexture_);
            }
            break;
        case State::Dragging:
            thumb_.setTexture(pressedTexture_);
            break;
        case State::Hover:
            if(isFocused())
            {
                thumb_.setTexture(focusTexture_);
            }
            else
            {
                thumb_.setTexture(hoverTexture_);
            }
            break;
    }

    track_.setPosition(RenderingPolicy::getTrackPosition(getGlobalPosition(), getSize()));
    thumb_.setPosition(RenderingPolicy::getThumbPosition(getGlobalPosition(), getSize(), track_.getSize(), thumb_.getSize(), normalizeValue()));
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::FocusLost&)
{
    updateTexture();
    return EventStatus::Consumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus Base<MouseHandlingPolicy, RenderingPolicy>::on(const event::FocusGained&)
{
    updateTexture();
    return EventStatus::Consumed;
}

template <typename MouseHandlingPolicy, typename RenderingPolicy>
void  Base<MouseHandlingPolicy, RenderingPolicy>::setThumbRatio(const float ratio)
{
    thumbRatio_ = ratio;
    setValue(getValue());
}

}  // namespace gui::slider
