#pragma once

#include <memory>

#include "gui/slider/Base.hpp"
#include "gui/slider/VerticalMousePolicy.hpp"
#include "gui/slider/VerticalThinRenderingPolicy.hpp"

namespace gui::slider
{

class Vertical : public Base<VerticalMousePolicy, VerticalThinRenderingPolicy>
{
public:
    static std::unique_ptr<Vertical> create();
};

}  // namespace gui::slider
