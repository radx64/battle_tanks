#include "gui/slider/Vertical.hpp"

namespace gui::slider
{

std::unique_ptr<Vertical> Vertical::create()
{
    return std::unique_ptr<Vertical>{new Vertical{}};
}

}  // namespace gui::slider
