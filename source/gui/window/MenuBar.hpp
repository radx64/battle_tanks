#pragma once

#include <vector>

#include <SFML/Graphics.hpp>

#include "gui/ContextMenu.hpp"
#include "gui/Widget.hpp"

namespace gui { class TextButton; }

namespace gui::window
{

class MenuBar : public Widget
{
public:
    MenuBar();

    void setItems(const std::vector<gui::ContextMenu::Item>& items);
    bool hasItems() const;
    float getPreferredHeight() const;
    void enable();
    void disable();

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

private:
    struct Entry
    {
        gui::ContextMenu::Item item;
        gui::TextButton* button = nullptr;
    };

    void rebuild();
    void layoutButtons();
    void openMenuForEntry(ptrdiff_t index);
    void closeOpenMenu();
    bool hasOpenMenu() const;

    std::vector<Entry> entries_;
    gui::ContextMenu* openMenu_ = nullptr;
    ptrdiff_t openMenuIndex_ = -1;
    sf::RectangleShape background_;
    sf::Color enabledColor_;
    sf::Color disabledColor_;
};

}  // namespace gui::window
