#pragma once

#include <SFML/Graphics.hpp>

#include "gui/TextDisplayModifier.hpp"

namespace gui {class Text;}

namespace gui
{

class TextCursor : public TextDisplayModifier
{
public:
    TextCursor(gui::Text& text);
    void setCharacterSize(uint32_t characterSize);
    void setFont(const sf::Font* font);

    void moveLeft(const bool moveWholeWord);
    void moveRight(const bool moveWholeWord);
    void moveTo(float mouse_x);

    void render(sf::RenderTexture& renderTexture);
    void update();
    uint32_t getIndex() const;
    void setIndex(const uint32_t index);
    sf::Vector2f getPosition();

    void disable();
    void enable();

protected:
    float getGlyphOffset(const std::string& string, const size_t index);

    uint32_t character_size_;
    const sf::Font* font_;
    gui::Text& text_;

    uint32_t text_length_;
    uint32_t cursor_index_;
    sf::RectangleShape cursor_;
    bool enabled_;
};

}  // namespace gui
