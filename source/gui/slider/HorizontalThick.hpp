#pragma once

#include <memory>

#include "gui/slider/Base.hpp"

#include "gui/slider/HorizontalMousePolicy.hpp"
#include "gui/slider/HorizontalThickRenderingPolicy.hpp"

namespace gui::slider
{

class HorizontalThick : public Base<HorizontalMousePolicy, HorizontalThickRenderingPolicy>
{
public:
    static std::unique_ptr<HorizontalThick> create();
};

}  // namespace gui::slider
