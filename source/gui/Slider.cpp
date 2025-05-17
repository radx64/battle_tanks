#include "gui/Slider.hpp"

#include <cmath>

#include "gui/TextureLibrary.hpp"

namespace gui
{

constexpr float THIN_TRACK_THICKNESS = 10.f;
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

float HorizontalMousePolicy::translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const float min, const float max, const float step, const sf::Vector2f& thumbSize, const sf::Vector2f& trackPosition)
{
    (void) sliderSize;
    (void) sliderPosition;
    (void) trackPosition;

    auto availableTruckRun = trackSize.x - thumbSize.x;

    auto thumbXPositionOffset = mousePosition.x  - trackPosition.x - thumbSize.x / 2.f;

    // Calculate screen step size
    float stepSize = availableTruckRun  / (max - min) * step;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbXPosition = std::round(thumbXPositionOffset / stepSize) * stepSize;
    // Normalize to 0.f to 1.f range
    float normalizedThumbXPosition = stepAlignedThumbXPosition / (availableTruckRun);
    // Map to set range
    float rangeMappedThumbXPosition = (max - min) * normalizedThumbXPosition + min;

    return std::clamp(rangeMappedThumbXPosition, min, max);
}

float VerticalMousePolicy::translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
    const sf::Vector2f& trackSize, const float min, const float max, const float step,  const sf::Vector2f& thumbSize, const sf::Vector2f& trackPosition)
{
    (void) thumbSize;
    (void) trackPosition;
    auto trackPositionOffset = sf::Vector2f{sliderSize.x / 2.f - THIN_TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    auto thumbYPositionOffset =  sliderPosition.y + trackSize.y - mousePosition.y + trackPositionOffset.y;

    float stepSize = trackSize.y  / (max - min) * step;

    // Calculate the nearest step by rounding instead of truncating
    float stepAlignedThumbYPosition = std::round(thumbYPositionOffset / stepSize) * stepSize;

    // Normalize to 0.f to 1.f range
    float normalizedThumbYPosition = stepAlignedThumbYPosition / trackSize.y;

    // Map to set range
    float rangeMappedThumbYPosition = (max - min) * normalizedThumbYPosition + min;

    return std::clamp(rangeMappedThumbYPosition, min, max);
}

sf::Vector2f HorizontalThinPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x - BOUNDS_THICKNESS * 2.f, THIN_TRACK_THICKNESS};
}

sf::Vector2f HorizontalThinPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    auto trackPositionOffset = sf::Vector2f{BOUNDS_THICKNESS, sliderSize.y / 2.f - THIN_TRACK_THICKNESS / 2.f};
    return sliderPosition + trackPositionOffset;
}

std::string HorizontalThinPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

sf::Vector2f HorizontalThinPolicy::getThumbSize(const sf::Vector2f&)
{
    return sf::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS};
}


sf::Vector2f VerticalThinPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{THIN_TRACK_THICKNESS, silderSize.y - BOUNDS_THICKNESS * 2.f};
}

sf::Vector2f VerticalThinPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    auto trackPositionOffset = sf::Vector2f{sliderSize.x / 2.f - THIN_TRACK_THICKNESS / 2.f, BOUNDS_THICKNESS};
    return sliderPosition + trackPositionOffset;
}

std::string VerticalThinPolicy::getTrackTextureName()
{
    return std::string{"slider_track"};
}

sf::Vector2f VerticalThinPolicy::getThumbSize(const sf::Vector2f&)
{
    return sf::Vector2f{THUMB_THICKNESS, THUMB_THICKNESS};
}

sf::Vector2f HorizontalThickPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x, silderSize.y};
}

std::string HorizontalThickPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

sf::Vector2f HorizontalThickPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    (void) sliderSize;
    return sliderPosition;
}

sf::Vector2f HorizontalThickPolicy::getThumbSize(const sf::Vector2f& sliderSize)
{
    return sf::Vector2f{THUMB_THICKNESS, sliderSize.y};
}

sf::Vector2f VerticalThickPolicy::getTrackSize(const sf::Vector2f& silderSize)
{
    return sf::Vector2f{silderSize.x, silderSize.y};
}

std::string VerticalThickPolicy::getTrackTextureName()
{
    return std::string{"slider_track_thick"};
}

sf::Vector2f VerticalThickPolicy::getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize)
{
    (void) sliderSize;
    return sliderPosition;
}   

sf::Vector2f VerticalThickPolicy::getThumbSize(const sf::Vector2f& sliderSize)
{
    return sf::Vector2f{sliderSize.x, THUMB_THICKNESS};
}


template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::SliderBase()
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

    track_.setTexture(TextureLibrary::instance().get(RenderingPolicy::getTrackTextureName()));

    thumb_.setTexture(normalTexture_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(track_);
    renderTexture.draw(thumb_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::onValueChange(std::function<void(float)> onValueChangeCallback)
{
    onValueChange_ = onValueChangeCallback;
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::setRange(const float min, const float max)
{
    min_ = min;
    max_ = max;
    setValue(getValue());
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::setValue(const float value)
{
    value_ = std::clamp(value, min_, max_);
    updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
float SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::getValue() const
{
    return value_;
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::setStep(const float step)
{
    step_ = step;
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::increase()
{
    setValue(getValue() + step_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::decrease()
{
    setValue(getValue() - step_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::onSizeChange()
{
    track_.setSize(RenderingPolicy::getTrackSize(getSize()));

    auto thumbSize = RenderingPolicy::getThumbSize(getSize());
    thumb_.setSize(thumbSize);
    thumb_.setOrigin(thumbSize / 2.f);
    updateTexture();
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::onPositionChange()
{
    updateTexture();
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
float SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::normalizeValue()
{
    return (value_ - min_) / (max_ - min_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
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

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
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

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseMoved& mouseMovedEvent)
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

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
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

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::processMovement(sf::Vector2f& mousePosition)
{
    value_ = MouseHandlingPolicy::translatePositionToThumbValue(mousePosition, getGlobalPosition(), getSize(),
        track_.getSize(), min_, max_, step_, thumb_.getSize(), track_.getPosition());

    focus();
    updateTexture();
    if (onValueChange_) onValueChange_(value_);
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
void SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::updateTexture()
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

    static_cast<SliderSpec*>(this)->updateTextureSpecific();
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::FocusLost&)
{
    updateTexture();
    return EventStatus::Consumed;
}

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
EventStatus SliderBase<SliderSpec, MouseHandlingPolicy, RenderingPolicy>::on(const event::FocusGained&)
{
    updateTexture();
    return EventStatus::Consumed;
}

void HorizontalSlider::updateTextureSpecific()
{
    auto trackSize = track_.getSize();
    //auto thumbSize = thumb_.getSize();

    auto thumbXPositionOffset = normalizeValue() * (trackSize.x);

    auto thumbYPositionOffset = getSize().y / 2.f;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

void VerticalSlider::updateTextureSpecific()
{
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

std::unique_ptr<HorizontalThickSlider> HorizontalThickSlider::create()
{
    return std::unique_ptr<HorizontalThickSlider>{new HorizontalThickSlider{}};
}

std::unique_ptr<VerticalThickSlider> VerticalThickSlider::create()
{
    return std::unique_ptr<VerticalThickSlider>{new VerticalThickSlider{}};
}

void HorizontalThickSlider::updateTextureSpecific()
{
    auto trackSize = track_.getSize();
    auto thumbSize = thumb_.getSize();

    auto thumbXPositionOffset = normalizeValue() * (trackSize.x - thumbSize.x) + thumbSize.x / 2.f;

    auto thumbYPositionOffset = getSize().y / 2.f;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

void VerticalThickSlider::updateTextureSpecific()
{
    auto trackSize = track_.getSize();

    auto thumbXPositionOffset = getSize().x / 2.f;
    auto thumbYPositionOffset = (1.f - normalizeValue()) * (trackSize.y) + BOUNDS_THICKNESS;
    
    auto thumbPositionOffset = sf::Vector2f{thumbXPositionOffset, thumbYPositionOffset};
    thumb_.setPosition(getGlobalPosition() + thumbPositionOffset);
}

template class SliderBase<HorizontalSlider, HorizontalMousePolicy, HorizontalThinPolicy>;
template class SliderBase<VerticalSlider, VerticalMousePolicy, VerticalThinPolicy>;
template class SliderBase<HorizontalThickSlider, HorizontalMousePolicy, HorizontalThickPolicy>;
template class SliderBase<VerticalThickSlider, VerticalMousePolicy, VerticalThickPolicy>;

}  // namespace gui
