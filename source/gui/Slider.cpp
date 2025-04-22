#include "gui/Slider.hpp"

#include <cmath>

#include "engine/math/Math.hpp"


namespace gui
{

constexpr float TRACK_THICKNESS = 5.f;
constexpr float THUMB_THICKNESS = 20.f;
constexpr float BOUNDS_THICKNESS = 10.f;
constexpr float STEP = 0.05f;

template <typename SliderSpec>
SliderBase<SliderSpec>::SliderBase()
: value_{0.0f}
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

template <typename SliderSpec>
void SliderBase<SliderSpec>::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(track_);
    renderTexture.draw(thumb_);
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::onValueChange(std::function<void(float)> onValueChangeCallback)
{
    onValueChange_ = onValueChangeCallback;
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::onSizeChange()
{
    track_.setSize(static_cast<SliderSpec*>(this)->getTrackSize());
    static_cast<SliderSpec*>(this)->updateTexture();
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::onPositionChange()
{
    static_cast<SliderSpec*>(this)->updateTexture();
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
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

    value_ = std::clamp(value_, 0.f, 1.f);
    static_cast<SliderSpec*>(this)->updateTexture();
    if (onValueChange_) onValueChange_(value_);

    return EventStatus::Consumed;
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
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

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::MouseMoved& mouseMovedEvent)
{
    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    if (state_ == State::Dragging)
    {
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
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

template <typename SliderSpec>
void SliderBase<SliderSpec>::processMovement(sf::Vector2f& mousePosition)
{
    value_ = static_cast<SliderSpec*>(this)->translateMousePositionToThumbValue(mousePosition);
    focus();
    static_cast<SliderSpec*>(this)->updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

float HorizontalSlider::translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    auto trackSize = track_.getSize();
    auto thumbXPositionOffset = mousePosition.x - getGlobalPosition().x - trackPositionOffset.x;

    // STEP below needs to be multiplied by track size as it varies depending on slider width
    float stepSize = STEP * trackSize.x;

    // Calculate the nearest step by rounding instead of truncating
    float alignedThumbXPosition = std::round(thumbXPositionOffset / stepSize) * stepSize;

    return std::clamp(alignedThumbXPosition / trackSize.x, 0.f, 1.f);
}

sf::Vector2f HorizontalSlider::getTrackSize()
{
    return sf::Vector2f{getSize().x - BOUNDS_THICKNESS * 2.f, TRACK_THICKNESS};
}

void HorizontalSlider::updateTexture()
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    track_.setPosition(getGlobalPosition() + trackPositionOffset);
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = value_ * (trackSize.x) + BOUNDS_THICKNESS;
    auto thumbYPositionOffset = getSize().y / 2.f;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

float VerticalSlider::translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const
{
    auto trackPositionOffset = sf::Vector2f{getSize().x / 2.f - TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    auto trackSize = track_.getSize();
    auto thumbYPositionOffset =  getGlobalPosition().y + trackSize.y - mousePosition.y + trackPositionOffset.y;

    // STEP below needs to be multiplied by track size as it varies depending on slider width
    float stepSize = STEP * trackSize.y;

    // Calculate the nearest step by rounding instead of truncating
    float alignedThumbYPosition = std::round(thumbYPositionOffset / stepSize) * stepSize;

    return std::clamp(alignedThumbYPosition / trackSize.y, 0.f, 1.f);
}

sf::Vector2f VerticalSlider::getTrackSize()
{
    return sf::Vector2f{TRACK_THICKNESS, getSize().y - BOUNDS_THICKNESS * 2.f};
}

void VerticalSlider::updateTexture()
{
    auto trackPositionOffset = sf::Vector2f{getSize().x / 2.f - TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    track_.setPosition(getGlobalPosition() + trackPositionOffset);
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = getSize().x / 2.f;
    auto thumbYPositionOffset = (1.f - value_) * (trackSize.y) + BOUNDS_THICKNESS;
    // TODO: this 1.f - value might be problematic when value range will be different than 0.f to 1.f
    // it should be calculated based on the range (when range is implemented)
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

std::unique_ptr<HorizontalSlider> HorizontalSlider::create()
{
    return std::unique_ptr<HorizontalSlider>{new HorizontalSlider{}};
}

std::unique_ptr<VerticalSlider> VerticalSlider::create()
{
    return std::unique_ptr<VerticalSlider>{new VerticalSlider{}};
}

template class SliderBase<VerticalSlider>;
template class SliderBase<HorizontalSlider>;

}  // namespace gui
