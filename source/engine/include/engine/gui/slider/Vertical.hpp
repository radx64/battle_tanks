#pragma once

#include <memory>

#include "engine/gui/slider/Base.hpp"
#include "engine/gui/slider/VerticalMousePolicy.hpp"
#include "engine/gui/slider/VerticalThinRenderingPolicy.hpp"

namespace engine::gui::slider
{

class Vertical : public Base<VerticalMousePolicy, VerticalThinRenderingPolicy>
{
public:
    static std::unique_ptr<Vertical> create();
};

}  // namespace engine::gui::slider
