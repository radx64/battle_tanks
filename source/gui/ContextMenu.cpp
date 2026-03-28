#include "gui/ContextMenu.hpp"

#include <algorithm>
#include <ranges>

#include "gui/Button.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/Text.hpp"
#include "gui/layout/Constraint.hpp"
#include "gui/layout/Vertical.hpp"


namespace gui
{
namespace
{
    constexpr float MENU_ENTRY_HEIGHT = 26.f;
    constexpr float MENU_ENTRY_MIN_WIDTH = 120.f;
    constexpr float HORIZONTAL_PADDING = 12.f;
    
    // TODO: implement rendering of arrows to submenus
    
    gui::FramedSprite::LayoutConfig::UVs buildUVsForMenu()
    {
        return gui::FramedSprite::LayoutConfig::UVs
        {
            .topLeft        = {0.0f,   0.0f,  2.0f, 2.0f},
            .topRight       = {4.0f,   0.0f,  2.0f, 2.0f},
            .bottomLeft     = {0.0f,   4.0f,  2.0f, 2.0f},
            .bottomRight    = {4.0f,   4.0f,  2.0f, 2.0f},
        };
    }

    gui::FramedSprite::LayoutConfig buildLayoutConfigForMenuTexture()
    {
        static auto layout = [] {
            gui::FramedSprite::LayoutConfig lc;
            lc.cornerSizes = {{4.f, 4.f}, {4.f, 4.f}};
            lc.uvs = buildUVsForMenu();
            return lc;
        }();
        return layout;
    }

    float calculateMenuWidth(const std::vector<ContextMenu::Item>& items)
    {
        auto style = BasicStyleSheetFactory::instance();

        gui::Text measure;
        measure.setFont(style.getFont());
        measure.setCharacterSize(style.getFontSize());

        float maxTextWidth = 0.f;
        for (auto& item : items)
        {
            measure.setText(item.text);
            maxTextWidth = std::max(maxTextWidth, measure.getTextBounds().width);
        }

        const float needed = maxTextWidth + HORIZONTAL_PADDING * 2.f;
        return std::max(needed, MENU_ENTRY_MIN_WIDTH);
    }

    sf::Vector2f globalToLocal(const Widget* widget, const sf::Vector2f& globalPosition)
    {
        if (widget == nullptr) return globalPosition;
        const auto parentGlobal = widget->getGlobalPosition();
        return globalPosition - parentGlobal;
    }

} // namespace

Overlay::Overlay()
: isDead_{false}
{ }

bool Overlay::isDead() const
{
    return isDead_;
}

void Overlay::markAsDead()
{
    isDead_ = true;
}

std::unique_ptr<ContextMenu> ContextMenu::create(const std::vector<Item>& items)
{
    return std::unique_ptr<ContextMenu>{new ContextMenu(items, nullptr)};
}

ContextMenu::ContextMenu(const std::vector<Item>& items, ContextMenu* parent)
: items_{items}
, parentMenu_{parent}
, openSubmenu_{}
, background_{buildLayoutConfigForMenuTexture()}
{
    // TODO: now I'm using buttons as menu items so menu background might not be visible at all
    // consider if this background it is needed later
    background_.setTexture(TextureLibrary::instance().get("button_normal"));
    setVisibility(false);
    buildMenu();
}

ContextMenu::~ContextMenu()
{
    closeSubmenu();
}

void ContextMenu::setCloseCallback(std::function<void(ContextMenu*)> callback)
{
    closeCallback_ = std::move(callback);
}

void ContextMenu::open(const sf::Vector2f& globalPosition)
{
    setPosition(globalToLocal(getParent(), globalPosition));
    setVisibility(true);
}

void ContextMenu::close()
{
    setVisibility(false);
    markAsDead();
    closeSubmenu();

    if (parentMenu_)
    {
        parentMenu_->onSubmenuClosed(this);
    }

    if (closeCallback_)
    {
        closeCallback_(this);
        return;
    }
    
    if (auto* parent = getParent())
    {
        parent->removeChild(this);
    }
}

void ContextMenu::onSubmenuClosed(ContextMenu* submenu)
{
    if (openSubmenu_.ptr == submenu)
    {
        openSubmenu_.ptr = nullptr;
    }
}

void ContextMenu::closeSubmenu()
{
    if (openSubmenu_.ptr)
    {
        openSubmenu_.ptr->close();
        openSubmenu_.ptr = nullptr;
    }
}

void ContextMenu::buildMenu()
{
    if (layout_)
    {
        removeChild(layout_);
        layout_ = nullptr;
    }

    if (items_.empty())
    {
        setSize({0.f, 0.f});
        return;
    }

    const float width = calculateMenuWidth(items_);
    const float height = static_cast<float>(items_.size()) * MENU_ENTRY_HEIGHT;

    setSize({width, height});

    auto vertical = gui::layout::Vertical::create(items_.size());
    vertical->setPosition({0.f, 0.f});
    vertical->setSize(getSize());
    vertical->setPadding(0);

    for (auto [index, item] : std::views::enumerate(items_))
    {
        vertical->setRowSize(index, gui::layout::Constraint::Pixels(MENU_ENTRY_HEIGHT));

        auto menuEntry = gui::TextButton::create(item.text);
        menuEntry->onClick([this, index, item, menuEntryPtr = menuEntry.get()]() mutable {
            if (not item.subItems.empty())
            {
                updateSubmenu(menuEntryPtr, item, index);
                return;
            }

            if (item.action)
            {
                item.action();
            }

            // Close the entire context menu hierarchy (not just this submenu)
            getRootMenu()->close();
        });

        menuEntry->onMouseEnter([this,index, item, menuEntryPtr = menuEntry.get()]() mutable {
            if (not item.subItems.empty())
            {
                updateSubmenu(menuEntryPtr, item, index);  
            }            
        });

        vertical->addChild(std::move(menuEntry));
    }

    layout_ = vertical.get();
    Widget::addChild(std::move(vertical));
}

void ContextMenu::updateSubmenu(gui::TextButton* buttonPtr, const Item& item, ptrdiff_t index)
{
    if (openSubmenu_.ptr &&  openSubmenu_.index == index) return;   // Do not respawn the same submenu if already spawned
    const auto itemGlobalPosition = buttonPtr->getGlobalPosition();
    const auto submenuPosition = sf::Vector2f{itemGlobalPosition.x + getSize().x, itemGlobalPosition.y};
    closeSubmenu();
    auto submenu = ContextMenu::create(item.subItems);
    openSubmenu_.index = index;
    openSubmenu_.ptr = submenu.get();
    openSubmenu_.ptr->parentMenu_ = this;
    addChild(std::move(submenu));
    openSubmenu_.ptr->open(submenuPosition);        
}

void ContextMenu::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

ContextMenu* ContextMenu::getDeepestOpenMenu()
{
    ContextMenu* current = this;
    while (current->openSubmenu_.ptr)
    {
        current = current->openSubmenu_.ptr;
    }
    return current;
}

ContextMenu* ContextMenu::getRootMenu()
{
    ContextMenu* root = this;
    while (root->parentMenu_)
    {
        root = root->parentMenu_;
    }
    return root;
}

EventStatus ContextMenu::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    const auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};

    bool isMouseInsideHierarchy = false;

    auto* activeMenu = getDeepestOpenMenu();

    while(activeMenu)
    {
        if (activeMenu->isInside(mousePosition))
        {
            isMouseInsideHierarchy = true;
            break;
        }
        activeMenu = activeMenu->parentMenu_;
    }

    if (!isMouseInsideHierarchy)
    {
        getRootMenu()->close();
    }

    return EventStatus::NotConsumed;
}

void ContextMenu::onPositionChange()
{
    background_.setPosition(getGlobalPosition());
}

void ContextMenu::onSizeChange()
{
    background_.setSize(getSize());

    if (layout_)
    {
        layout_->setSize(getSize());
    }
}

}  // namespace gui
