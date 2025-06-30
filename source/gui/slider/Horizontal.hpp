#pragma once

#include <memory>

#include "gui/slider/Base.hpp"

#include "gui/slider/HorizontalMousePolicy.hpp"
#include "gui/slider/HorizontalThinRenderingPolicy.hpp"

namespace gui::slider
{

class Horizontal : public Base<HorizontalMousePolicy, HorizontalThinRenderingPolicy>
{
public:
    static std::unique_ptr<Horizontal> create();
};

}  // namespace gui::slider
