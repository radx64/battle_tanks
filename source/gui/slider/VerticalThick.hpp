#pragma once

#include "gui/slider/Base.hpp"
#include "gui/slider/VerticalMousePolicy.hpp"
#include "gui/slider/VerticalThickRenderingPolicy.hpp"

namespace gui::slider
{

class VerticalThick: public Base<VerticalMousePolicy, VerticalThickRenderingPolicy>
{
public:
    static std::unique_ptr<VerticalThick> create();
};

}  // namespace gui::slider
