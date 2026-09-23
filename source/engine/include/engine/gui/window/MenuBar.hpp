#pragma once

#include <vector>

#include "engine/gui/ContextMenu.hpp"
#include "engine/gui/Widget.hpp"

#include "engine/gfx/Color.hpp"
#include "engine/gfx/Shape.hpp"

namespace engine::gui { class TextButton; }

namespace engine::gui::window
{

class MenuBar : public Widget
{
public:
    MenuBar();

    void setItems(const std::vector<engine::gui::ContextMenu::Item>& items);
    bool hasItems() const;
    float getPreferredHeight() const;
    void enable();
    void disable();

protected:
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onSizeChange() override;
    void onPositionChange() override;

private:
    struct Entry
    {
        engine::gui::ContextMenu::Item item;
        engine::gui::TextButton* button = nullptr;
    };

    void rebuild();
    void layoutButtons();
    void openMenuForEntry(ptrdiff_t index);
    void closeOpenMenu();
    bool hasOpenMenu() const;

    std::vector<Entry> entries_;
    engine::gui::ContextMenu* openMenu_ = nullptr;
    ptrdiff_t openMenuIndex_ = -1;
    engine::gfx::RectangleShape background_;
    engine::gfx::Color enabledColor_;
    engine::gfx::Color disabledColor_;
};

}  // namespace engine::gui::window
