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
    bool isOngoing() const;
    bool isEmpty() const;

    void start(const uint32_t index, const sf::Vector2f& position);
    size_t startsAt() const;

    void updateEnd(const uint32_t index, const sf::Vector2f& position);
    void end();
    size_t endsAt() const;

    size_t length() const;

    void clear();

    void update();

    void render(sf::RenderTexture& renderTexture) override;

protected:
    gui::Text& text_;
    
    size_t selection_start_index_;
    size_t selection_end_index_;

    bool is_ongoing_;
    sf::Vector2f selection_start_position_;
    sf::Vector2f selection_end_position_;

    sf::RectangleShape selection_;
};

}  // namespace gui
