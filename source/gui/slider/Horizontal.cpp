#include "gui/slider/Horizontal.hpp"

#include <memory>

namespace gui::slider
{
    
std::unique_ptr<Horizontal> Horizontal::create()
{
    return std::unique_ptr<Horizontal>{new Horizontal{}};
} 

}  // namespace gui::slider
