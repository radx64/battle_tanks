#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/Selection.hpp"
#include "gui/Text.hpp"
#include "gui/TextCursor.hpp"

namespace gui
{

class EditBox : public Component
{
public:
    static std::unique_ptr<EditBox> create();

    ~EditBox() = default;

    std::string getText();
    void setText(const std::string_view text);
    void setAlignment(const gui::Alignment& alignment);

protected:
    EditBox();
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
};

}  // namespace gui
