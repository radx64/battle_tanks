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

class Slider : public Component
{
public:
    static std::unique_ptr<Slider> create();

    void onRender(sf::RenderTexture& renderTexture) override;
    void onValueChange(std::function<void(float)> onValueChangeCallback);

protected:
    enum class State
    {
        Idle,
        Dragging,
    };

    Slider();

    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    void updateTexture();
    void processMovement(sf::Vector2f& mousePosition);
    float translateMousePositionToThumbValue(const sf::Vector2f& mousePosition) const;

    sf::RectangleShape track_;
    sf::RectangleShape thumb_;

    float value_;
    std::function<void(float)> onValueChange_;
    State state_;
};

}  // namespace gui
