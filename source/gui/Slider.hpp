#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"

namespace gui
{

/* TODO:
    - mouse wheel support
    - render steps marks (implement after focus frame rendering is done)
*/

template <typename SliderSpec, typename MouseHandlingPolicy, typename RenderingPolicy>
class SliderBase : public Component
{
public:
    void onValueChange(std::function<void(float)> onValueChangeCallback);

    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;
    void setStep(const float step);
    void increase();
    void decrease();

protected:
    enum class State
    {
        Idle,
        Dragging,
        Hover   // TODo add disabled in the future
    };

    void onRender(sf::RenderTexture& renderTexture) override;
    SliderBase();

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
    
    std::function<void(float)> onValueChange_;
    State state_;
};


class HorizontalMousePolicy
{
public:
    static float translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& trackPosition,
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step);
};

class VerticalMousePolicy
{
public:
    static float translatePositionToThumbValue(const sf::Vector2f& mousePosition, const sf::Vector2f& trackPosition,
    const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float min, const float max, const float step);
};

class HorizontalThinPolicy
{
public:
    static sf::Vector2f getTrackSize(const sf::Vector2f& silderSize);
    static sf::Vector2f getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static sf::Vector2f getThumbSize(const sf::Vector2f& sliderSize);
    static sf::Vector2f getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue);
};

class VerticalThinPolicy
{
public:
    static sf::Vector2f getTrackSize(const sf::Vector2f& silderSize);
    static sf::Vector2f getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static sf::Vector2f getThumbSize(const sf::Vector2f& sliderSize);
    static sf::Vector2f getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue);
};

class HorizontalThickPolicy
{
public:
    static sf::Vector2f getTrackSize(const sf::Vector2f& silderSize);
    static sf::Vector2f getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static sf::Vector2f getThumbSize(const sf::Vector2f& sliderSize);
    static sf::Vector2f getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue);
};

class VerticalThickPolicy
{
public:
    static sf::Vector2f getTrackSize(const sf::Vector2f& silderSize);
    static sf::Vector2f getTrackPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize);
    static std::string getTrackTextureName();

    static sf::Vector2f getThumbSize(const sf::Vector2f& sliderSize);
    static sf::Vector2f getThumbPosition(const sf::Vector2f& sliderPosition, const sf::Vector2f& sliderSize, 
        const sf::Vector2f& trackSize, const sf::Vector2f& thumbSize, const float normalizedValue);
};

class HorizontalSlider : public SliderBase<HorizontalSlider, HorizontalMousePolicy, HorizontalThinPolicy>
{
public:
    static std::unique_ptr<HorizontalSlider> create();
};

class VerticalSlider : public SliderBase<VerticalSlider, VerticalMousePolicy, VerticalThinPolicy>
{
public:
    static std::unique_ptr<VerticalSlider> create();
};

class HorizontalThickSlider : public SliderBase<HorizontalThickSlider, HorizontalMousePolicy, HorizontalThickPolicy>
{
public:
    static std::unique_ptr<HorizontalThickSlider> create();
};

class VerticalThickSlider : public SliderBase<VerticalThickSlider, VerticalMousePolicy, VerticalThickPolicy>
{
public:
    static std::unique_ptr<VerticalThickSlider> create();
};


}  // namespace gui
