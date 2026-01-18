#pragma once

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/Selection.hpp"
#include "gui/Text.hpp"
#include "gui/TextCursor.hpp"

namespace gui
{

class MultiLineEditBox : public Component
{
public:
    static std::unique_ptr<MultiLineEditBox> create();

    ~MultiLineEditBox() = default;

    std::string getText();
    void setText(const std::string_view text);
    void setAlignment(const gui::Alignment& alignment);

protected:
    MultiLineEditBox();
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonDoublePressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleased) override;
    EventStatus on(const event::TextEntered& textEntered) override;

    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

    void updateTextVisbleArea();
    void updateCursorPosition();

    void enterEdit();
    void cut();
    void copy();
    void paste();
    void startSelection();
    void updateCursorAndSelection(const bool atSelectionEndOnCancel);

    // Line management
    struct LineInfo
    {
        size_t startIndex;  // Index in the full text string
        size_t length;      // Length of this line (excluding newline)
    };

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

    gui::FramedSprite background_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    gui::Text text_;
    gui::TextCursor textCursor_;
    gui::Selection selection_;
    uint32_t maxLength_;
    bool anyShiftHeldDown_;
    bool mouseLeftButtonPressed_;
    gui::Alignment alignment_;
    
    std::vector<LineInfo> lineIndices_;
    uint32_t maxLines_;
};

}  // namespace gui
