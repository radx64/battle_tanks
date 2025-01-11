#pragma once

#include <SFML/Graphics.hpp>

#include "engine/Timer.hpp"

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
    void moveTo(float mouseX);

    void render(sf::RenderTexture& renderTexture);
    void update();
    uint32_t getIndex() const;
    void setIndex(const uint32_t index);
    sf::Vector2f getPosition();

    void disable();
    void enable();

protected:
    float getGlyphOffset(const std::string& string, const size_t index);
    void animateCursor();
    void startBlinkAnimation();
    void stopBlinkAnimation();

    uint32_t characterSize_;
    const sf::Font* font_;
    gui::Text& text_;

    uint32_t textLength_;
    uint32_t cursorIndex_;
    sf::RectangleShape cursorImage_;
    bool enabled_;
    bool isCursorVisible_;

    engine::Timer blinkTimer_;
};

}  // namespace gui
