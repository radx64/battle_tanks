#pragma once

#include <cstddef>

#include <SFML/Graphics.hpp>

#include "engine/gfx/Color.hpp"
#include "engine/gfx/Shape.hpp"
#include "engine/Vector2.hpp"
#include "engine/gfx/RenderTarget.hpp"

#include "engine/gui/TextDisplayModifier.hpp"

namespace engine::gui {class Text;}

namespace engine::gui
{

class Selection : public TextDisplayModifier
{
public:
    Selection(engine::gui::Text& text, const engine::gfx::Color& selectionColor);
    bool isEmpty() const;

    void start(const uint32_t index, const engine::Vec2f& position);
    size_t startsAt() const;

    void to(const uint32_t index, const engine::Vec2f& position);
    size_t endsAt() const;
    size_t length() const;

    void clear();
    void update();
    void render(engine::gfx::RenderTarget& renderTarget) override;

protected:
    engine::gui::Text& text_;
    
    size_t selectionStartIndex_;
    size_t selectionEndIndex_;

    engine::Vec2f selectionStartPosition_;
    engine::Vec2f selectionEndPosition_;

    std::vector<engine::gfx::RectangleShape> selectionRectangles_;
    engine::gfx::Color selectionColor_;

private:
    size_t getLineFromIndex(const std::string& text, size_t index) const;
    size_t getColumnFromIndex(const std::string& text, size_t index) const;
    size_t getLineStartIndex(const std::string& text, size_t lineNumber) const;
    size_t getLineEndIndex(const std::string& text, size_t lineNumber) const;
    float getLineXWidth(const std::string& text, size_t lineStartIdx, size_t lineEndIdx) const;
};

}  // namespace engine::gui
