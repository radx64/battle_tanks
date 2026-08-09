#pragma once

#include <memory>

#include <SFML/Graphics.hpp>

#include "engine/gui/Alignment.hpp"
#include "engine/gui/FramedSprite.hpp"
#include "engine/gui/IScrollableWidget.hpp"
#include "engine/gui/Selection.hpp"
#include "engine/gui/style/EditBox.hpp"
#include "engine/gui/Text.hpp"
#include "engine/gui/TextCursor.hpp"
#include "engine/gui/Widget.hpp"

namespace engine::gui
{

class BaseEditBox : public IScrollableWidget
{
public:
    ~BaseEditBox();

    std::string getText();
    void setText(const std::string_view text);
    void setAlignment(const engine::gui::Alignment& alignment);

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

}  // namespace engine::gui
