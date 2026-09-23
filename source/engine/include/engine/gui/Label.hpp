#pragma once

#include <memory>

#include "engine/gfx/Color.hpp"
#include "engine/gui/Alignment.hpp"
#include "engine/gui/Widget.hpp"
#include "engine/gui/Text.hpp"
#include "engine/Rect.hpp"

namespace engine::gui::style { class Text; }

namespace engine::gui
{

class Label : public Widget
{
public:
    static std::unique_ptr<Label> create(const std::string_view& text);
    static std::unique_ptr<Label> create(const std::string_view& text, const style::Text& style);

    void setText(const std::string_view& text);
    void setAlignment(const engine::gui::Alignment& alignment);
    void setFontColor(const engine::gfx::Color& color);
    engine::FloatRect getTextBounds() const;

protected:
    Label(const std::string_view& text);
    Label(const std::string_view& text, const style::Text& style);
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onSizeChange() override;
    void onPositionChange() override;
    void recalculatePositionAndSize();
    engine::gui::Alignment alignment_;
    engine::gui::Text text_;
};

}  // namespace engine::gui
