#pragma once

#include <functional>
#include <memory>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>

#include "gui/Component.hpp"
#include "gui/FramedSprite.hpp"

namespace gui {class TextButton;}

namespace gui
{

class ContextMenu : public Component
{
public:
    struct Item
    {
        Item() = default;
        Item(std::string text_, std::function<void()> action_ = {}, std::vector<Item> subItems_ = {})
            : text(std::move(text_))
            , action(std::move(action_))
            , subItems(std::move(subItems_))
        {
        }

        std::string text;
        std::function<void()> action;
        std::vector<Item> subItems;
    };

    // TODO: refactor Item and SubmenuEntity later, they might be used when implementing
    // other menu types like dropdowns, toolbars, etc.
    struct SubmenuEntity
    {
        ptrdiff_t index;
        ContextMenu* ptr;
    };

    static std::unique_ptr<ContextMenu> create(const std::vector<Item>& items);
    ~ContextMenu() override;
    
    void open(const sf::Vector2f& globalPosition);
    void close();
    
    void setCloseCallback(std::function<void(ContextMenu*)> callback);
    
protected:
    explicit ContextMenu(const std::vector<Item>& items, ContextMenu* parent = nullptr);
    
    void onRender(sf::RenderTexture& renderTexture) override;
    
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonReleased& mouseButtonReleasedEvent) override;

    void onPositionChange() override;
    void onSizeChange() override;
    void buildMenu();
    void closeSubmenu();
    void onSubmenuClosed(ContextMenu* submenu);
    
    ContextMenu* getDeepestOpenMenu();
    ContextMenu* getRootMenu();

    void updateSubmenu(gui::TextButton* buttonPtr, const Item& item, ptrdiff_t index);

    std::vector<Item> items_;
    Component* layout_ = nullptr;
    ContextMenu* parentMenu_;
    SubmenuEntity openSubmenu_;

    std::function<void(ContextMenu*)> closeCallback_;

    gui::FramedSprite background_;
};

}  // namespace gui
