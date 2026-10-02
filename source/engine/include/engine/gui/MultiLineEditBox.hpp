#pragma once

#include <functional>
#include <memory>
#include <vector>


#include "engine/gui/BaseEditBox.hpp"
#include "engine/Vector2.hpp"

namespace engine::gui
{

class MultiLineEditBox : public BaseEditBox
{
public:
    static std::unique_ptr<MultiLineEditBox> create();

    ~MultiLineEditBox() = default;

    engine::Vec2f getContentSize() override;
    size_t getLineCount() const;
    size_t getCursorLine() const;

protected:
    MultiLineEditBox();

    void onSizeChange() override;
    void updateTextVisbleArea() override;
    void onTextChanged() override;

    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent) override;
    EventStatus on(const engine::events::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const engine::events::KeyPressed& keyboardKeyPressed) override;
    EventStatus on(const engine::events::TextEntered& textEntered) override;

private:
    void ensureCursorVisible();

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
    size_t getIndexFromScreenPosition(const engine::Vec2f& screenPos);

    // Line management
    struct LineInfo
    {
        size_t startIndex;  // Index in the full text string
        size_t length;      // Length of this line (excluding newline)
    };

    std::vector<LineInfo> lineIndices_;
};

}  // namespace engine::gui
