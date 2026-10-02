#pragma once


#include "engine/gui/Widget.hpp"

#include "engine/gfx/RenderTarget.hpp"

namespace engine::gui::layout
{

class Base : public Widget
{
public:
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
};

}  // namespace engine::gui::layout
