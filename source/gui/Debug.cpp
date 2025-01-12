#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{

static bool debug_enabled{false};

void toggle()
{
    debug_enabled = !debug_enabled;
}

void drawBounds(sf::RenderTarget& renderTarget, const Component* component)
{
    if (not debug_enabled) return;

    sf::RectangleShape bound_rect;
    bound_rect.setSize(component->getSize());
    bound_rect.setPosition(component->getGlobalPosition());
    if (component->isFocused())
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

std::string getFunctionNameOnly(const std::source_location& location)
{
    auto functionName = std::string{location.function_name()};
    functionName = functionName.substr(0, functionName.find('('));
    functionName = functionName.substr(functionName.find_last_of(':') + 1, functionName.size());
    return functionName;
}

}  // namespace gui::debugq
