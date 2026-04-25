#pragma once

#include <cstddef>
#include <functional>
#include <memory>
#include <optional>
#include <string>
#include <vector>

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/RenderTexture.hpp>
#include <SFML/Graphics/Sprite.hpp>

#include "gui/FramedSprite.hpp"
#include "gui/Widget.hpp"
#include "gui/style/EditBox.hpp"
#include "gui/style/Text.hpp"

namespace gui::scrollbar { class Vertical; }

namespace gui
{

class ListBox : public Widget
{
public:
    struct Item
    {
        Item() = default;
        Item(std::string text_, std::function<void()> onClick_ = {})
            : text(std::move(text_))
            , onClick(std::move(onClick_))
        {
        }

        std::string text;
        std::function<void()> onClick;
    };

    static std::unique_ptr<ListBox> create();

    ListBox();

    void setItems(const std::vector<Item>& items);
    void addItem(const Item& item);
    void clear();

    const std::vector<Item>& getItems() const;

    void setSelectedIndex(std::optional<std::size_t> index);
    std::optional<std::size_t> getSelectedIndex() const;

    void onSelectionChange(std::function<void(std::optional<std::size_t>)> callback);

protected:
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void onPositionChange() override;

    EventStatus on(const event::MouseMoved& mouseMovedEvent) override;
    EventStatus on(const event::MouseButtonPressed& mouseButtonPressedEvent) override;
    EventStatus on(const event::MouseLeft&) override;
    EventStatus on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent) override;
    EventStatus on(const event::FocusLost&) override;
    EventStatus on(const event::FocusGained&) override;

private:
    void updateLayout();
    void updateScrollbar();
    void syncSelection();
    void scrollSelectionIntoView();

    float getViewportWidth() const;
    float getViewportHeight() const;
    float getItemHeight() const;
    std::size_t getVisibleItemCapacity() const;
    std::size_t getVisibleItemSpan() const;
    std::size_t getMaxFirstVisibleIndex() const;
    std::size_t getFirstVisibleIndex() const;
    std::optional<std::size_t> getItemIndexAt(const sf::Vector2f& globalPosition) const;

    void notifySelectionChange();
    void activateItem(std::size_t index);

    style::EditBox style_;
    FramedSprite background_;
    const sf::Texture& focusTexture_;
    const sf::Texture& normalTexture_;
    const style::Text& textStyle_;
    sf::RenderTexture contentTexture_;
    sf::Sprite contentSprite_;

    scrollbar::Vertical* scrollbar_;
    std::vector<Item> items_;
    std::function<void(std::optional<std::size_t>)> onSelectionChange_;

    std::optional<std::size_t> selectedIndex_;
    std::optional<std::size_t> hoveredIndex_;
    float scrollValue_;
};

}  // namespace gui
