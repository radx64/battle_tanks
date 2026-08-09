#include "engine/gui/slider/HorizontalThick.hpp"

namespace engine::gui::slider
{
    
std::unique_ptr<HorizontalThick> HorizontalThick::create()
{
    return std::unique_ptr<HorizontalThick>{new HorizontalThick{}};
}

}  // namespace engine::gui::slider
