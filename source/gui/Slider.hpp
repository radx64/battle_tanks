#pragma once

#include <functional>
#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui
{

/* TODO

- ranges instead of ange 0.f to 1.f
- discrete step
- mouse wheel support
- skinning

*/

template <typename SliderSpec>
class SliderBase : public Component
{
public:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onValueChange(std::function<void(float)> onValueChangeCallback);

    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;
    void setStep(const float step);

protected:
    enum class State
    {
        Idle,
        Dragging,
    };

    SliderBase();

    void onSizeChange() override;
    void onPositionChange() override;

    float normalizeValue();

    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    void processMovement(sf::Vector2f& mousePosition);

    sf::RectangleShape track_;
    sf::RectangleShape thumb_;

    float value_;
    float step_;
    float min_;
    float max_;
    
    std::function<void(float)> onValueChange_;
    State state_;
};

class HorizontalSlider : public SliderBase<HorizontalSlider>
{
public:
    static std::unique_ptr<HorizontalSlider> create();

    float translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const;
    sf::Vector2f getTrackSize();
    void updateTexture();

};

class VerticalSlider : public SliderBase<VerticalSlider>
{
public:
    static std::unique_ptr<VerticalSlider> create();

    float translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const;
    sf::Vector2f getTrackSize();
    void updateTexture();
};

}  // namespace gui
