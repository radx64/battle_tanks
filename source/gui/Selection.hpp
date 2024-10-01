#pragma once

#include <cstddef>

#include <SFML/Graphics.hpp>

#include "gui/TextDisplayModifier.hpp"

namespace gui {class Text;}

namespace gui
{

class Selection : public TextDisplayModifier
{
public:
    Selection(gui::Text& text);
    bool isActive() const;
    bool isEmpty() const;

    void start(const uint32_t index, const sf::Vector2f& position);
    size_t startsAt() const;

    void to(const uint32_t index, const sf::Vector2f& position);
    void end();
    size_t endsAt() const;

    size_t length() const;

    void clear();

    void update();

    void render(sf::RenderTexture& renderTexture) override;

protected:
    gui::Text& text_;
    
    size_t selectionStartIndex_;
    size_t selectionEndIndex_;

    bool isActive_;
    sf::Vector2f selectionStartPosition_;
    sf::Vector2f selectionEndPosition_;

    sf::RectangleShape selection_;
};

}  // namespace gui
