#include "gui/ContextMenu.hpp"

#include "gui/Button.hpp"
#include "gui/StyleSheet.hpp"
#include "gui/TextureLibrary.hpp"
#include "gui/Text.hpp"
#include "gui/layout/Constraint.hpp"
#include "gui/layout/Vertical.hpp"

#include <algorithm>

namespace gui
{

namespace
{
    constexpr float kItemHeight = 26.f;
    constexpr float kMinWidth = 120.f;
    constexpr float kHorizontalPadding = 12.f;
    constexpr float kSubmenuArrowPadding = 16.f;  // TODO: implement rendering of arrows to submenus

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

        const float needed = maxTextWidth + kHorizontalPadding * 2.f + kSubmenuArrowPadding;
        return std::max(needed, kMinWidth);
    }

    sf::Vector2f globalToLocal(const Component* component, const sf::Vector2f& globalPosition)
    {
        if (component == nullptr) return globalPosition;
        const auto parentGlobal = component->getGlobalPosition();
        return globalPosition - parentGlobal;
    }

} // namespace

std::unique_ptr<ContextMenu> ContextMenu::create(const std::vector<Item>& items)
{
    return std::unique_ptr<ContextMenu>{new ContextMenu(items, nullptr)};
}

ContextMenu::ContextMenu(const std::vector<Item>& items, ContextMenu* parent)
: items_(items)
, parentMenu_(parent)
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

    // Ensure children underneath the menu stop thinking the mouse is over them.
    if (auto* parent = getParent())
    {
        for (auto& sibling : parent->getChildren())
        {
            if (sibling.get() != this)
            {
                sibling->forceMouseLeave();
            }
        }
    }
}

void ContextMenu::close()
{
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
    if (openSubmenu_ == submenu)
    {
        openSubmenu_ = nullptr;
    }
}

void ContextMenu::closeSubmenu()
{
    if (openSubmenu_)
    {
        openSubmenu_->close();
        openSubmenu_ = nullptr;
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
    const float height = static_cast<float>(items_.size()) * kItemHeight;

    setSize({width, height});

    auto vertical = gui::layout::Vertical::create(items_.size());
    vertical->setPosition({0.f, 0.f});
    vertical->setSize(getSize());
    vertical->setPadding(0);

    for (size_t i = 0; i < items_.size(); ++i)
    {
        vertical->setRowSize(i, gui::layout::Constraint::Pixels(kItemHeight));

        auto item = items_[i];
        auto button = gui::TextButton::create(item.text);

        button->onClick([this, item, buttonPtr = button.get()]() mutable {
            // If item has submenu, open it
            if (!item.subItems.empty())
            {
                const auto itemGlobalPosition = buttonPtr->getGlobalPosition();
                const auto submenuPosition = sf::Vector2f{itemGlobalPosition.x + getSize().x, itemGlobalPosition.y};
                closeSubmenu();

                auto submenu = ContextMenu::create(item.subItems);
                openSubmenu_ = submenu.get();
                openSubmenu_->parentMenu_ = this;
                addChild(std::move(submenu));
                openSubmenu_->open(submenuPosition);
                return;
            }

            if (item.action)
            {
                item.action();
            }

            // Close the entire context menu hierarchy (not just this submenu)
            getRootMenu()->close();
        });

        vertical->addChild(std::move(button));
    }

    layout_ = vertical.get();
    Component::addChild(std::move(vertical));
}

void ContextMenu::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);
}

ContextMenu* ContextMenu::getDeepestOpenMenu()
{
    ContextMenu* current = this;
    while (current->openSubmenu_)
    {
        current = current->openSubmenu_;
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

    auto* activeMenu = getDeepestOpenMenu();

    if (!activeMenu->isInside(mousePosition))
    {
        //TODO: add check if click happens in menu hierarchy so you can still call the parent's actions instead of closing menu
        getRootMenu()->close();
    }

    return EventStatus::Consumed;
}

EventStatus ContextMenu::on(const event::MouseButtonReleased&)
{
    return EventStatus::Consumed;
}

EventStatus ContextMenu::on(const event::MouseMoved&)
{
    return EventStatus::Consumed;
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
