#pragma once

#include <memory>
#include <functional>

#include "engine/gui/Widget.hpp"

#include <SFML/Graphics.hpp>

namespace engine::gui
{

class Checkbox : public Widget
{
public:
    static std::unique_ptr<Checkbox> create(const bool isChecked);

    void onRender(sf::RenderTexture& renderTexture) override;
    void onStateChange(std::function<void(bool)> onClickCallback);
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
    void recalculateSpritesPositions();

    enum class State
    {
        Normal,
        Hover, // TODO add disabled in the future
    };

    EventStatus on(const engine::events::KeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const engine::gui::events::MouseEntered& mouseEnteredEvent) override;
    EventStatus on(const engine::gui::events::MouseLeft& mouseLeftEvent) override;

    EventStatus on(const engine::gui::events::FocusLost&) override;
    EventStatus on(const engine::gui::events::FocusGained&) override;

    void onPositionChange() override;
    void onSizeChange() override;

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

}  // namespace engine::gui
