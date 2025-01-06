#include <source_location>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"

namespace gui::debug
{
    void toggle();

    void drawBounds(sf::RenderTexture& renderWindow, const Component* component);

    std::string getFunctionNameOnly(const std::source_location& location);

}  // namespace gui::debug
