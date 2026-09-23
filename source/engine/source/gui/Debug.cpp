#include "engine/gui/Widget.hpp"

#include "engine/gfx/Color.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/Vector2.hpp"

namespace engine::gui::debug
{

static bool debug_enabled{false};

void toggle()
{
    debug_enabled = !debug_enabled;
}

void drawBounds(engine::gfx::RenderTarget& renderTarget, const Widget* widget)
{
    if (not debug_enabled) return;

    engine::gfx::RectangleShape bound_rect;

    const auto widget_size = widget->getSize();
    const auto widget_position = widget->getGlobalPosition();

    bound_rect.setSize(widget_size);
    bound_rect.setPosition(widget_position);
    if (widget->isFocused())
    {
        bound_rect.setFillColor(engine::gfx::Color{255,255,0,127});
        bound_rect.setOutlineColor(engine::gfx::Color::Yellow);
        bound_rect.setOutlineThickness(4.f);
    }
    else
    {
        bound_rect.setFillColor(engine::gfx::Color::Transparent);
        bound_rect.setOutlineColor(engine::gfx::Color::Magenta);
        bound_rect.setOutlineThickness(2.f);
    }
    renderTarget.draw(bound_rect);
}
void drawBox(engine::gfx::RenderTarget& renderTarget, const engine::Vec2f position, 
    const engine::Vec2f size, const engine::gfx::Color color, const float thickness)
{
    if (not debug_enabled) return;

    engine::gfx::RectangleShape box;
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(engine::gfx::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(thickness);

    renderTarget.draw(box);
}

std::string getFunctionNameOnly(const std::source_location& location)
{
    auto functionName = std::string{location.function_name()};
    functionName = functionName.substr(0, functionName.find('('));
    functionName = functionName.substr(functionName.find_last_of(':') + 1, functionName.size());
    return functionName;
}

}  // namespace engine::gui::debugq
