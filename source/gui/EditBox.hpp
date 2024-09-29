#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/Selection.hpp"
#include "gui/Text.hpp"
#include "gui/TextCursor.hpp"

namespace gui
{

class EditBox : public Component
{
public:
    EditBox();
    ~EditBox();

    std::string getText();

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;

    EventStatus on(const event::KeyboardKeyPressed& keyboardKeyPressed) override;
    EventStatus on(const event::TextEntered& textEntered) override;

    void onFocus() override;
    void onFocusLost() override; 

    void updateTextVisbleArea();

    void copy();
    void paste();
    void toggleSelection(const bool enable);
    void startSelection();
    void endSelection();
    void updateCursorAndSelection(const size_t cursorIndexOnSelectionCancel);

    sf::RectangleShape background_;
    gui::Text text_;
    gui::TextCursor textCursor_;
    gui::Selection selection_;
    uint32_t maxLength_;
};

}  // namespace gui
