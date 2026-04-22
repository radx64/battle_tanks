#include "gui/window/MenuBar.hpp"

#include <algorithm>
#include <ranges>
#include <utility>

#include "gui/Button.hpp"
#include "gui/GUI.hpp"
#include "gui/Label.hpp"
#include "gui/style/StyleFactory.hpp"

namespace gui::window
{
namespace
{
    constexpr float MENU_BUTTON_MIN_WIDTH = 54.f;
    constexpr float MENU_BUTTON_HORIZONTAL_PADDING = 16.f;
    constexpr float MENU_BUTTON_VERTICAL_PADDING = 20.f;

    float calculateButtonWidth(const std::string& text)
    {
        const auto& style = gui::style::StyleFactory::instance().button.text;
        auto label = gui::Label::create(text, style);
        const auto neededWidth = label->getTextBounds().width + MENU_BUTTON_HORIZONTAL_PADDING * 2.f;
        return std::max(neededWidth, MENU_BUTTON_MIN_WIDTH);
    }

    float calculateButtonHeight()
    {
        const auto& style = gui::style::StyleFactory::instance().button.text;
        return static_cast<float>(style.size) + MENU_BUTTON_VERTICAL_PADDING;
    }
}

MenuBar::MenuBar()
: enabledColor_{sf::Color{192, 192, 192}}
, disabledColor_{sf::Color{170, 170, 170}}
{
    background_.setFillColor(enabledColor_);
}

void MenuBar::setItems(const std::vector<gui::ContextMenu::Item>& items)
{
    closeOpenMenu();

    entries_.clear();
    entries_.reserve(items.size());
    for (const auto& item : items)
    {
        entries_.push_back(Entry{.item = item});
    }

    setVisibility(not entries_.empty());
    rebuild();
}

bool MenuBar::hasItems() const
{
    return not entries_.empty();
}

float MenuBar::getPreferredHeight() const
{
    return calculateButtonHeight();
}

void MenuBar::enable()
{
    background_.setFillColor(enabledColor_);
}

void MenuBar::disable()
{
    background_.setFillColor(disabledColor_);
    closeOpenMenu();
}

void MenuBar::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

void MenuBar::onSizeChange()
{
    background_.setSize(getSize());
    layoutButtons();
}

void MenuBar::onPositionChange()
{
    background_.setPosition(getGlobalPosition());
}

void MenuBar::rebuild()
{
    while (hasChildren())
    {
        removeChild(getChildren().back().get());
    }

    for (auto [index, entry] : std::views::enumerate(entries_))
    {
        // TODO: menu bar and context menu should use different styles,
        // as menu bar buttons should be flat and pop up on hover
        // and context menu buttons should be highlited with color
        auto button = gui::TextButton::create(entry.item.text, style::StyleFactory::instance().flatButton);
        button->setSize({calculateButtonWidth(entry.item.text), calculateButtonHeight()});
        button->disableFocus();
        button->onClick([this, index]() {
            if (openMenu_ && openMenuIndex_ == static_cast<ptrdiff_t>(index))
            {
                closeOpenMenu();
                return;
            }

            openMenuForEntry(index);
        });
        button->onMouseEnter([this, index]() {
            if (hasOpenMenu())
            {
                openMenuForEntry(index);
            }
        });

        entry.button = button.get();
        Widget::addChild(std::move(button));
    }

    layoutButtons();
}

void MenuBar::layoutButtons()
{
    float offsetX = 0.f;

    for (auto& entry : entries_)
    {
        if (!entry.button)
        {
            continue;
        }

        entry.button->setPosition({offsetX, 0.f});
        entry.button->setSize({entry.button->getSize().x, getSize().y});
        offsetX += entry.button->getSize().x;
    }
}

void MenuBar::openMenuForEntry(ptrdiff_t index)
{
    if (index < 0 || index >= static_cast<ptrdiff_t>(entries_.size()))
    {
        return;
    }

    auto& entry = entries_[index];
    if (!entry.button)
    {
        return;
    }

    closeOpenMenu();

    if (entry.item.subItems.empty())
    {
        if (entry.item.action)
        {
            entry.item.action();
        }
        return;
    }

    auto menu = gui::ContextMenu::create(entry.item.subItems);
    openMenu_ = menu.get();
    openMenuIndex_ = index;
    menu->setOnClose([this](gui::ContextMenu* menuPtr) {
        if (openMenu_ == menuPtr)
        {
            openMenu_ = nullptr;
            openMenuIndex_ = -1;
        }
    });

    const auto menuPosition = entry.button->getGlobalPosition() + sf::Vector2f{0.f, entry.button->getSize().y};
    gui().openContextMenu(std::move(menu), menuPosition);
}

void MenuBar::closeOpenMenu()
{
    if (!openMenu_)
    {
        return;
    }

    auto* menu = openMenu_;
    openMenu_ = nullptr;
    openMenuIndex_ = -1;
    menu->close();
}

bool MenuBar::hasOpenMenu() const
{
    return openMenu_ != nullptr;
}

}  // namespace gui::window
