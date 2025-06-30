#include "gui/slider/HorizontalThick.hpp"

namespace gui::slider
{
    
std::unique_ptr<HorizontalThick> HorizontalThick::create()
{
    return std::unique_ptr<HorizontalThick>{new HorizontalThick{}};
}

}  // namespace gui::slider
