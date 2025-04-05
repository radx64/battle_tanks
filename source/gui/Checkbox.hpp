#pragma once

#include <memory>
#include <functional>

#include "gui/Component.hpp"

#include <SFML/Graphics.hpp>

namespace gui
{

class Checkbox : public Component
{
public:
    static std::unique_ptr<Checkbox> create(const bool isChecked);

    void onRender(sf::RenderTexture& renderTexture) override;
    void onClick(std::function<void(bool)> onClickCallback);
    void setState(const bool checked);
    bool isChecked() const;
    
protected:
    Checkbox(const bool isChecked,
        const sf::Texture& normalTexture,
        const sf::Texture& hoverTexture,
        const sf::Texture& focusTexture,
        const sf::Texture& checkedTexture);

    Checkbox(const bool isChecked);
    
    void updateTexture();

    enum class State
    {
        Normal,
        Hover, // TODO add disabled in the future
    };

    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const event::MouseLeft& mouseLeftEvent) override;

    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

    void onPositionChange() override;

    bool isChecked_ = false;
    std::function<void(bool)> onClick_;
    State state_;
    sf::Sprite background_;
    sf::Sprite tick_;
    const sf::Texture& hoverTexture_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    const sf::Texture& checkedTexture_;

};


}  // namespace gui
