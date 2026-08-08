#pragma once
#include <SFML/Graphics.hpp>

#include "engine/gui/Widget.hpp"
#include "engine/gui/FramedSprite.hpp"

namespace engine::gui { class Label; }

namespace engine::gui
{

class ProgressBar : public Widget
{
public:
    static std::unique_ptr<ProgressBar> create();
    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;

protected:
    ProgressBar();
    void setTextLabel(const float value);
    void onPositionChange() override;
    void onSizeChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    void recalculateSize();

    float min_{};
    float max_{};
    float value_{};
    float progress_{};

    engine::gui::FramedSprite background_;
    const sf::Texture& normalTexture_;

    sf::RectangleShape bar_;
    engine::gui::Label* text_;
};

}  // namespace
