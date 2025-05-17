#include "gui/slider/VerticalThick.hpp"

namespace gui::slider
{

std::unique_ptr<VerticalThick> VerticalThick::create()
{
    return std::unique_ptr<VerticalThick>{new VerticalThick{}};
}

}  // namespace gui::slider
