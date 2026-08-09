#pragma once

#include "engine/gui/slider/Base.hpp"
#include "engine/gui/slider/VerticalMousePolicy.hpp"
#include "engine/gui/slider/VerticalThickRenderingPolicy.hpp"

namespace engine::gui::slider
{

class VerticalThick: public Base<VerticalMousePolicy, VerticalThickRenderingPolicy>
{
public:
    static std::unique_ptr<VerticalThick> create();
};

}  // namespace engine::gui::slider
