#include "engine/gui/slider/Vertical.hpp"

namespace engine::gui::slider
{

std::unique_ptr<Vertical> Vertical::create()
{
    return std::unique_ptr<Vertical>{new Vertical{}};
}

}  // namespace engine::gui::slider
