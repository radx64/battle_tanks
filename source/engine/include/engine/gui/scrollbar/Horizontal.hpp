#pragma once

#include <memory>

#include "engine/gui/scrollbar/Base.hpp"
#include "engine/gui/slider/HorizontalThick.hpp"

namespace engine::gui::scrollbar
{

class Horizontal : public Base
{
public:
    static std::unique_ptr<Horizontal> create();
    void setThumbRatio(const float ratio);
    void setValue(float value);
    float getValue() const;
    
protected:
    Horizontal();
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    engine::gui::slider::HorizontalThick* slider_ptr_;
};

} // namespace engine::gui::scrollbar
