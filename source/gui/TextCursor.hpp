#pragma once
#include <SFML/Graphics.hpp>

namespace gui {class Text;}

namespace gui
{

class TextCursor
{
public:
    TextCursor(const gui::Text& text);
    void setCharacterSize(uint32_t characterSize);
    void setFont(const sf::Font* font);

    void moveLeft();
    void moveRight();
    void moveTo(float mouse_x);

    void render(sf::RenderTexture& renderTexture);
    void update();
    uint32_t getIndex() const;
    void setIndex(const uint32_t index);
    sf::Vector2f getGlobalPosition();

protected:
    float getGlyphSizeAt(const std::string& string, const size_t index);

    uint32_t character_size_;
    const sf::Font* font_;
    const gui::Text& text_;

    uint32_t text_length_;
    uint32_t cursor_index_;
    sf::RectangleShape cursor_;
};

}  // namespace gui