#pragma once

#include <memory>

#include "engine/gui/slider/Base.hpp"

#include "engine/gui/slider/HorizontalMousePolicy.hpp"
#include "engine/gui/slider/HorizontalThickRenderingPolicy.hpp"

namespace engine::gui::slider
{

class HorizontalThick : public Base<HorizontalMousePolicy, HorizontalThickRenderingPolicy>
{
public:
    static std::unique_ptr<HorizontalThick> create();
};

}  // namespace engine::gui::slider
