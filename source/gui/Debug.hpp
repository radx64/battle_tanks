#include <source_location>

#include <SFML/Graphics.hpp>

#include "gui/Widget.hpp"

namespace gui::debug
{
    void toggle();

    void drawBounds(sf::RenderTarget& renderTarget, const Widget* widget);

    void drawBox(sf::RenderTarget& renderTarget, const sf::Vector2f position, 
        const sf::Vector2f size, const sf::Color color, const float thickness);

    std::string getFunctionNameOnly(const std::source_location& location);

}  // namespace gui::debug
