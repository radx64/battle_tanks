#include "gui/Slider.hpp"

#include <cmath>

#include "gui/TextureLibrary.hpp"

namespace gui
{

constexpr float TRACK_THICKNESS = 10.f;
constexpr float THUMB_THICKNESS = 20.f;
constexpr float BOUNDS_THICKNESS = 10.f;


gui::FramedSprite::LayoutConfig buildLayoutConfig(const sf::Vector2f& cornerSizes, const gui::FramedSprite::LayoutConfig::UVs& uvs)
{
    gui::FramedSprite::LayoutConfig layoutConfig{
        .cornerSizes = 
        {
            .topLeft        = {cornerSizes.x, cornerSizes.y},
            .bottomRight    = {cornerSizes.x, cornerSizes.y}
        },
        .uvs = uvs
    };

    return layoutConfig;
} 

gui::FramedSprite::LayoutConfig::UVs buildUVsForTrackTexture()
{
    return gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

gui::FramedSprite::LayoutConfig buildLayoutConfigForTrackTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForTrackTexture());
    return layout;
}


gui::FramedSprite::LayoutConfig::UVs buildUVsForThumbTexture()
{
    return gui::FramedSprite::LayoutConfig::UVs
    {
        .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
        .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
        .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
        .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
    };
}

gui::FramedSprite::LayoutConfig buildLayoutConfigForThumbTexture()
{
    static auto layout = buildLayoutConfig({4.f, 4.f}, buildUVsForThumbTexture());
    return layout;
}

template <typename SliderSpec>
SliderBase<SliderSpec>::SliderBase()
: track_ {buildLayoutConfigForTrackTexture()}
, thumb_ {buildLayoutConfigForThumbTexture()}
, hoverTexture_{TextureLibrary::instance().get("button_hover")}
, focusTexture_{TextureLibrary::instance().get("button_focus")}
, normalTexture_{TextureLibrary::instance().get("button_normal")}
, pressedTexture_{TextureLibrary::instance().get("button_pressed")} // reusing button textures for now
, value_{0.0f}
, step_{0.01f}
, min_{0.0f}
, max_{1.0f}
, onValueChange_{nullptr}
, state_{State::Idle}
{
    enableFocus();

    track_.setTexture(TextureLibrary::instance().get("slider_track"));
    track_.setSize(sf::Vector2f{TRACK_THICKNESS, TRACK_THICKNESS});

    thumb_.setTexture(normalTexture_);
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
void SliderBase<SliderSpec>::setRange(const float min, const float max)
{
    min_ = min;
    max_ = max;
    value_ = std::clamp(value_, min_, max_);
    updateTextureGeneral();
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::setValue(const float value)
{
    value_ = std::clamp(value, min_, max_);
    updateTextureGeneral();
    if (onValueChange_) onValueChange_(value_);
}

template <typename SliderSpec>
float SliderBase<SliderSpec>::getValue() const
{
    return value_;
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::setStep(const float step)
{
    step_ = step;
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::onSizeChange()
{
    track_.setSize(static_cast<SliderSpec*>(this)->getTrackSize());
    updateTextureGeneral();
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::onPositionChange()
{
    updateTextureGeneral();
}

template <typename SliderSpec>
float SliderBase<SliderSpec>::normalizeValue()
{
    return (value_ - min_) / (max_ - min_);
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
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

    updateTextureGeneral();

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
        state_ = State::Dragging;
        processMovement(mousePosition);
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
    else
    {
        bool isMouseInside = isInside(mousePosition);
        if (isMouseInside and state_ != State::Hover and not isFocused())
        {
            state_ = State::Hover;
            updateTextureGeneral();
            return EventStatus::Consumed;
        }
        else if (not isMouseInside and state_ == State::Hover)
        {
            state_ = State::Idle;
            updateTextureGeneral();
            return EventStatus::Consumed;
        }
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
        state_ = State::Idle;
        processMovement(mousePosition);
        return EventStatus::Consumed;
    }
    return EventStatus::NotConsumed;
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::processMovement(sf::Vector2f& mousePosition)
{
    value_ = static_cast<SliderSpec*>(this)->translateMousePositionToThumbValue(mousePosition);
    focus();
    updateTextureGeneral();
    if (onValueChange_) onValueChange_(value_);
}

template <typename SliderSpec>
void SliderBase<SliderSpec>::updateTextureGeneral()
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
            thumb_.setTexture(hoverTexture_);
            break;
    }

    static_cast<SliderSpec*>(this)->updateTextureSpecific();
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::FocusLost&)
{
    updateTextureGeneral();
    return EventStatus::Consumed;
}

template <typename SliderSpec>
EventStatus SliderBase<SliderSpec>::on(const event::FocusGained&)
{
    updateTextureGeneral();
    return EventStatus::Consumed;
}

float HorizontalSlider::translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    auto trackSize = track_.getSize();
    auto thumbXPositionOffset = mousePosition.x - getGlobalPosition().x - trackPositionOffset.x;

    // Calculate screen step size
    float stepSize = trackSize.x  / (max_ - min_) * step_;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbXPosition = std::round(thumbXPositionOffset / stepSize) * stepSize;
    // Normalize to 0.f to 1.f range
    float normalizedThumbXPosition = stepAlignedThumbXPosition / trackSize.x;
    // Map to set range
    float rangeMappedThumbXPosition = (max_ - min_) * normalizedThumbXPosition + min_;   

    return std::clamp(rangeMappedThumbXPosition, min_, max_);
}

sf::Vector2f HorizontalSlider::getTrackSize()
{
    return sf::Vector2f{getSize().x - BOUNDS_THICKNESS * 2.f, TRACK_THICKNESS};
}

void HorizontalSlider::updateTextureSpecific()
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, getSize().y / 2.f - TRACK_THICKNESS / 2.f};
    track_.setPosition(getGlobalPosition() + trackPositionOffset);
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = normalizeValue() * (trackSize.x) + BOUNDS_THICKNESS;
    auto thumbYPositionOffset = getSize().y / 2.f;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

float VerticalSlider::translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const
{
    auto trackPositionOffset = sf::Vector2f{getSize().x / 2.f - TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    auto trackSize = track_.getSize();
    auto thumbYPositionOffset =  getGlobalPosition().y + trackSize.y - mousePosition.y + trackPositionOffset.y;

    float stepSize = trackSize.y  / (max_ - min_) * step_;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbYPosition = std::round(thumbYPositionOffset / stepSize) * stepSize;

    // Normalize to 0.f to 1.f range
    float normalizedThumbYPosition = stepAlignedThumbYPosition / trackSize.y;

    // Map to set range
    float rangeMappedThumbYPosition = (max_ - min_) * normalizedThumbYPosition + min_;

    return std::clamp(rangeMappedThumbYPosition, min_, max_);
}

sf::Vector2f VerticalSlider::getTrackSize()
{
    return sf::Vector2f{TRACK_THICKNESS, getSize().y - BOUNDS_THICKNESS * 2.f};
}

void VerticalSlider::updateTextureSpecific()
{
    auto trackPositionOffset = sf::Vector2f{getSize().x / 2.f - TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    track_.setPosition(getGlobalPosition() + trackPositionOffset);
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = getSize().x / 2.f;
    auto thumbYPositionOffset = (1.f - normalizeValue()) * (trackSize.y) + BOUNDS_THICKNESS;
    
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
