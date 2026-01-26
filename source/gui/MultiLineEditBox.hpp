#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "gui/BaseEditBox.hpp"

namespace gui
{

class MultiLineEditBox : public BaseEditBox
{
public:
    static std::unique_ptr<MultiLineEditBox> create();

    ~MultiLineEditBox() = default;

protected:
    MultiLineEditBox();

    void updateTextVisbleArea() override;
    void onTextChanged() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
    EventStatus on(const event::TextEntered& textEntered) override;

private:
    void updateCursorPosition();

    void rebuildLineIndices();
    void insertNewLine();
    void deleteNewLine();
    void moveCursorUp();
    void moveCursorDown();
    void moveCursorHome();
    void moveCursorEnd();
    size_t getLineFromIndex(size_t textIndex) const;
    size_t getColumnFromIndex(size_t textIndex) const;
    size_t getIndexFromLineColumn(size_t line, size_t column) const;
    size_t getIndexFromScreenPosition(const sf::Vector2f& screenPos);

    // Line management
    struct LineInfo
    {
        size_t startIndex;  // Index in the full text string
        size_t length;      // Length of this line (excluding newline)
    };

    std::vector<LineInfo> lineIndices_;
    uint32_t maxLines_;
};

}  // namespace gui
