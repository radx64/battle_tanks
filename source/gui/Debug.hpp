#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{
    void draw_bounds(sf::RenderTexture& renderWindow, const Component* component);
}  // namespace gui::debug