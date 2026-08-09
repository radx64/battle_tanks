#include "engine/gui/slider/VerticalThick.hpp"

namespace engine::gui::slider
{

std::unique_ptr<VerticalThick> VerticalThick::create()
{
    return std::unique_ptr<VerticalThick>{new VerticalThick{}};
}

}  // namespace engine::gui::slider
