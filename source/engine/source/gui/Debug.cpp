#include <SFML/Graphics.hpp>

#include "engine/gui/Widget.hpp"

namespace engine::gui::debug
{

static bool debug_enabled{false};

void toggle()
{
    debug_enabled = !debug_enabled;
}

void drawBounds(sf::RenderTarget& renderTarget, const Widget* widget)
{
    if (not debug_enabled) return;

    sf::RectangleShape bound_rect;

    const auto widget_size = widget->getSize();
    const auto widget_position = widget->getGlobalPosition();

    bound_rect.setSize(sf::Vector2f{widget_size.x, widget_size.y});
    bound_rect.setPosition(sf::Vector2f{widget_position.x, widget_position.y});
    if (widget->isFocused())
    {
        bound_rect.setFillColor(sf::Color(255,255,0,127));
        bound_rect.setOutlineColor(sf::Color::Yellow);
        bound_rect.setOutlineThickness(4.f);
    }
    else
    {
        bound_rect.setFillColor(sf::Color::Transparent);
        bound_rect.setOutlineColor(sf::Color::Magenta);
        bound_rect.setOutlineThickness(2.f);
    }
    renderTarget.draw(bound_rect);
}
void drawBox(sf::RenderTarget& renderTarget, const sf::Vector2f position, 
    const sf::Vector2f size, const sf::Color color, const float thickness)
{
    if (not debug_enabled) return;

    sf::RectangleShape box;
    box.setSize(size);
    box.setPosition(position);
    box.setFillColor(sf::Color::Transparent);
    box.setOutlineColor(color);
    box.setOutlineThickness(thickness);

    renderTarget.draw(box);
}

void drawBox(sf::RenderTarget& renderTarget, const engine::Vec2f position, 
    const engine::Vec2f size, const sf::Color color, const float thickness)
{
   drawBox(renderTarget, sf::Vector2f{position.x, position.y}, sf::Vector2f{size.x, size.y}, color, thickness); 
}

std::string getFunctionNameOnly(const std::source_location& location)
{
    auto functionName = std::string{location.function_name()};
    functionName = functionName.substr(0, functionName.find('('));
    functionName = functionName.substr(functionName.find_last_of(':') + 1, functionName.size());
    return functionName;
}

}  // namespace engine::gui::debugq
