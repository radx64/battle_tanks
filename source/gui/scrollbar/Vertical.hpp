#pragma once

#include <memory>

#include "gui/scrollbar/Base.hpp"
#include "gui/slider/VerticalThick.hpp"

namespace gui::scrollbar
{

class Vertical : public Base
{
public:
    static std::unique_ptr<Vertical> create();
    void setThumbRatio(const float ratio);

protected:
    Vertical();
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(sf::RenderTexture& renderTexture) override;

    gui::slider::VerticalThick* slider_ptr_;
};

} // namespace gui::scrollbar
