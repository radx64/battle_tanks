#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{

void draw_bounds(sf::RenderTexture& renderWindow, const Component* component)
{
    sf::RectangleShape bound_rect;
    bound_rect.setSize(component->getSize());
    bound_rect.setPosition(component->getGlobalPosition());
    bound_rect.setFillColor(sf::Color::Transparent);
    bound_rect.setOutlineColor(sf::Color::Magenta);
    bound_rect.setOutlineThickness(2.f);
    renderWindow.draw(bound_rect);
}

}  // namespace gui::debug