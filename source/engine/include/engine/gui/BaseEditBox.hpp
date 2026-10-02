#pragma once

#include <memory>


#include "engine/Vector2.hpp"
#include "engine/gfx/Texture.hpp"

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

    engine::Vec2f getContentSize() override;
    void applyOffset(const engine::Vec2f& offset) override;

protected:
    BaseEditBox();
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const engine::events::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::gui::events::MouseButtonDoublePressed& mouseButtonPressedEvent) override;
    EventStatus on(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent) override;
    EventStatus on(const engine::events::MouseMoved& mouseMovedEvent) override;

    EventStatus on(const engine::events::KeyPressed& keyboardKeyPressed) override;
    EventStatus on(const engine::events::KeyReleased& keyboardKeyReleased) override;
    EventStatus on(const engine::events::TextEntered& textEntered) override;

    EventStatus on(const engine::gui::events::FocusLost&) override;
    EventStatus on(const engine::gui::events::FocusGained&) override;

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
    const engine::gfx::Texture& focusTexture_;
    const engine::gfx::Texture& normalTexture_;
    Text text_;
    TextCursor textCursor_;
    Selection selection_;
    uint32_t maxLength_;
    bool anyShiftHeldDown_;
    bool mouseLeftButtonPressed_;
    Alignment alignment_;
};

}  // namespace engine::gui
