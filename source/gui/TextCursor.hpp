#pragma once
#include <SFML/Graphics.hpp>

namespace gui {class Text;}

namespace gui
{

class TextCursor
{
public:
    TextCursor();
    void setCharacterSize(uint32_t characterSize);
    void setFont(const sf::Font* font);

    void moveLeft();
    void moveRight();
    void moveTo(const gui::Text& text, float mouse_x);

    void render(sf::RenderTexture& renderTexture);

    void update(const gui::Text& text);


    uint32_t getIndex() const;

protected:
    uint32_t character_size_;
    const sf::Font* font_;

    uint32_t text_length_;
    uint32_t cursor_index_;
    sf::RectangleShape cursor_;
};

}  // namespace gui