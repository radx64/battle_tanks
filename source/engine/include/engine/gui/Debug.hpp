#include <source_location>

#include "engine/gui/Widget.hpp"

#include "engine/gfx/Color.hpp"
#include "engine/gfx/RenderTarget.hpp"
#include "engine/Vector2.hpp"


namespace engine::gui::debug
{
    void toggle();

    void drawBounds(engine::gfx::RenderTarget& renderTarget, const Widget* widget);

    void drawBox(engine::gfx::RenderTarget& renderTarget, const engine::Vec2f position, 
        const engine::Vec2f size, const engine::gfx::Color color, const float thickness);

    std::string getFunctionNameOnly(const std::source_location& location);

}  // namespace engine::gui::debug
