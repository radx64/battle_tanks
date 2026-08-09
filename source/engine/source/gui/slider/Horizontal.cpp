#include "engine/gui/slider/Horizontal.hpp"

#include <memory>

namespace engine::gui::slider
{
    
std::unique_ptr<Horizontal> Horizontal::create()
{
    return std::unique_ptr<Horizontal>{new Horizontal{}};
} 

}  // namespace engine::gui::slider
