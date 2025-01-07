#pragma once

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
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
    ~EditBox() = default;

    std::string getText();
    void setText(const std::string_view text);
    void setAlignment(const gui::Alignment& alignment);

protected:
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

    void enterEdit();
    void cut();
    void copy();
    void paste();
    void startSelection();
    void updateCursorAndSelection(const bool atSelectionEndOnCancel);

    gui::Text text_;
    sf::RectangleShape backgroundShape_;
    gui::TextCursor textCursor_;
    gui::Selection selection_;
    uint32_t maxLength_;
    bool anyShiftHeldDown_;
    bool mouseLeftButtonPressed_;
    gui::Alignment alignment_;
};

}  // namespace gui
