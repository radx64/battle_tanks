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
        Hover   // TODo add disabled in the future
    };

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
    void updateTextureGeneral();

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

class HorizontalSlider : public SliderBase<HorizontalSlider>
{
public:
    static std::unique_ptr<HorizontalSlider> create();

    float translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const;
    sf::Vector2f getTrackSize();
    void updateTextureSpecific();

};

class VerticalSlider : public SliderBase<VerticalSlider>
{
public:
    static std::unique_ptr<VerticalSlider> create();

    float translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const;
    sf::Vector2f getTrackSize();
    void updateTextureSpecific();
};

}  // namespace gui
