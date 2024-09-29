#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{
    void toggle();

    void drawBounds(sf::RenderTexture& renderWindow, const Component* component);

}  // namespace gui::debug
