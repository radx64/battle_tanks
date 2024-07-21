#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui { class Label; }

namespace gui
{

class ProgressBar : public Component
{
public:
    ProgressBar();
    void setRange(const float min, const float max);
    void setValue(const float value);
    float getValue() const;

protected:
    void setTextLabel(const float value);
    void onPositionChange() override;
    void onSizeChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    float min_{};
    float max_{};
    float value_{};
    float progress_{};
    sf::RectangleShape background_;
    sf::RectangleShape bar_;
    gui::Label* text_;
};

}  // namespace