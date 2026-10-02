#pragma once


#include "engine/Timer.hpp"

#include "engine/gui/TextDisplayModifier.hpp"

#include "engine/gfx/Font.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/Vector2.hpp"
#include "engine/Rect.hpp"

namespace engine::gui {class Text;}

namespace engine::gui
{

class TextCursor : public TextDisplayModifier
{
public:
    TextCursor(engine::gui::Text& text);
    void setCharacterSize(uint32_t characterSize);
    void setFont(const engine::gfx::Font* font);

    void moveLeft(const bool moveWholeWord);
    void moveRight(const bool moveWholeWord);
    void moveTo(float mouseX);

    void render(engine::gfx::RenderTarget& renderTarget) override;
    void update();
    uint32_t getIndex() const;
    void setIndex(const uint32_t index);
    engine::Vec2f getPosition() const;
    engine::FloatRect getBounds() const;

    void disable();
    void enable();

protected:
    float getGlyphOffset(const std::string& string, const size_t index);
    void animateCursor();
    void startBlinkAnimation();
    void stopBlinkAnimation();
    
    // Helper methods for multiline text support
    size_t getLineFromIndex(const std::string& text, size_t index) const;
    size_t getColumnFromIndex(const std::string& text, size_t index) const;
    float getLineHeight() const;

    uint32_t characterSize_;
    const engine::gfx::Font* font_;
    engine::gui::Text& text_;

    uint32_t textLength_;
    uint32_t cursorIndex_;
    engine::gfx::RectangleShape cursorImage_;
    bool enabled_;
    bool isCursorVisible_;

    engine::Timer blinkTimer_;
};

}  // namespace engine::gui
