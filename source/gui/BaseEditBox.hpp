#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "gui/Alignment.hpp"
#include "gui/FramedSprite.hpp"
#include "gui/IScrollableWidget.hpp"
#include "gui/Selection.hpp"
#include "gui/style/EditBox.hpp"
#include "gui/Text.hpp"
#include "gui/TextCursor.hpp"
#include "gui/Widget.hpp"

namespace gui
{

class BaseEditBox : public IScrollableWidget
{
public:
    ~BaseEditBox();

    std::string getText();
    void setText(const std::string_view text);
    void setAlignment(const gui::Alignment& alignment);

    sf::Vector2f getViewportSize() override;
    sf::Vector2f getContentSize() override;
    void applyOffset(const sf::Vector2f& offset) override;

protected:
    BaseEditBox();
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

    // Virtual methods for subclasses to override
    virtual void updateTextVisbleArea() = 0;
    virtual void onTextChanged() {}

    void enterEdit();
    void cut();
    void copy();
    void paste();
    void startSelection();
    void updateCursorAndSelection(const bool atSelectionEndOnCancel);

    style::EditBox style_;
    FramedSprite background_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    Text text_;
    TextCursor textCursor_;
    Selection selection_;
    uint32_t maxLength_;
    bool anyShiftHeldDown_;
    bool mouseLeftButtonPressed_;
    Alignment alignment_;

};

}  // namespace gui
