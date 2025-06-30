#pragma once

#include <memory>

#include "gui/scrollbar/Base.hpp"
#include "gui/slider/HorizontalThick.hpp"

namespace gui::scrollbar
{

class Horizontal : public Base
{
public:
    static std::unique_ptr<Horizontal> create();
    void setThumbRatio(const float ratio);
    
protected:
    Horizontal();
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    gui::slider::HorizontalThick* slider_ptr_;
    float ratio_;
};

} // namespace gui::scrollbar
