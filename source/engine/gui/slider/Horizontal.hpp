#pragma once

#include <memory>

#include "engine/gui/slider/Base.hpp"

#include "engine/gui/slider/HorizontalMousePolicy.hpp"
#include "engine/gui/slider/HorizontalThinRenderingPolicy.hpp"

namespace engine::gui::slider
{

class Horizontal : public Base<HorizontalMousePolicy, HorizontalThinRenderingPolicy>
{
public:
    static std::unique_ptr<Horizontal> create();
};

}  // namespace engine::gui::slider
