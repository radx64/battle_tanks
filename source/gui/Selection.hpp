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
    bool isEmpty() const;

    void start(const uint32_t index, const sf::Vector2f& position);
    size_t startsAt() const;

    void to(const uint32_t index, const sf::Vector2f& position);
    size_t endsAt() const;
    size_t length() const;

    void clear();
    void update();
    void render(sf::RenderTexture& renderTexture) override;

protected:
    gui::Text& text_;
    
    size_t selectionStartIndex_;
    size_t selectionEndIndex_;

    sf::Vector2f selectionStartPosition_;
    sf::Vector2f selectionEndPosition_;

    std::vector<sf::RectangleShape> selectionRectangles_;

private:
    size_t getLineFromIndex(const std::string& text, size_t index) const;
    size_t getColumnFromIndex(const std::string& text, size_t index) const;
    size_t getLineStartIndex(const std::string& text, size_t lineNumber) const;
    size_t getLineEndIndex(const std::string& text, size_t lineNumber) const;
    float getLineXWidth(const std::string& text, size_t lineStartIdx, size_t lineEndIdx) const;
};

}  // namespace gui
