#pragma once

#include <cstddef>

#include <SFML/Graphics.hpp>

namespace gui {class Text;}

namespace gui
{


// FIXME: When selected text offset changes (moves) selection is not moving along with it
// I can't store selection position from cursor data as these are not valid anymore
// Need to recalculate it every update to be sure these are up to date.

class Selection
{
public:
    Selection(const gui::Text& text);
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
    void render(sf::RenderTexture& renderTexture);

protected:
    const gui::Text& text_;
    bool is_ongoing_;
    size_t selection_start_index_;
    size_t selection_end_index_;

    sf::Vector2f selection_start_position_;
    sf::Vector2f selection_end_position_;

    sf::RectangleShape selection_;

};

}  // namespace gui