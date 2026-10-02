#pragma once


#include "engine/gui/Widget.hpp"

namespace engine::gui::window
{

class Panel : public Widget
{
public:
    Panel();

    void enable();
    void disable();

    void addChild(std::unique_ptr<Widget> child) override;

protected:
    void onSizeChange() override;
    void onPositionChange() override;
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
};

}  // namespace engine::gui::window
