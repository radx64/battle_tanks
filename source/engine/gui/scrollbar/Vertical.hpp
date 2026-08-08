#pragma once

#include <memory>

#include "engine/gui/scrollbar/Base.hpp"
#include "engine/gui/slider/VerticalThick.hpp"

namespace engine::gui::scrollbar
{

class Vertical : public Base
{
public:
    static std::unique_ptr<Vertical> create();
    void setValue(float value);
    float getValue() const;
    void setStep(float step);
    void setThumbRatio(const float ratio);

protected:
    Vertical();
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    engine::gui::slider::VerticalThick* slider_ptr_;
};

} // namespace engine::gui::scrollbar
