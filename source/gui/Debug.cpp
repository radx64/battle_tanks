#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{

static bool debug_enabled{false};

void toggle()
{
    debug_enabled = !debug_enabled;
}

void drawBounds(sf::RenderTexture& renderWindow, const Component* component)
{
    if (not debug_enabled) return;
    
    sf::RectangleShape bound_rect;
    bound_rect.setSize(component->getSize());
    bound_rect.setPosition(component->getGlobalPosition());
    bound_rect.setFillColor(sf::Color::Transparent);
    bound_rect.setOutlineColor(sf::Color::Magenta);
    bound_rect.setOutlineThickness(2.f);
    renderWindow.draw(bound_rect);
}

}  // namespace gui::debug
