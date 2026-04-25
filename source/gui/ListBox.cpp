#include "gui/ListBox.hpp"

#include <algorithm>
#include <cmath>

#include "gui/FontHeightCache.hpp"
#include "gui/FontLibrary.hpp"
#include "gui/scrollbar/Vertical.hpp"
#include "gui/style/StyleFactory.hpp"

namespace
{

constexpr auto INNER_PADDING = 6.f;
constexpr auto ROW_HORIZONTAL_PADDING = 8.f;
constexpr auto ROW_VERTICAL_PADDING = 4.f;
constexpr auto SCROLLBAR_WIDTH = 32.f;
constexpr auto SCROLLBAR_SPACING = 4.f;

gui::FramedSprite::LayoutConfig buildLayoutConfigForListBoxTexture()
{
    return gui::FramedSprite::LayoutConfig{
        .cornerSizes = {.topLeft = {4.f, 4.f}, .bottomRight = {4.f, 4.f}},
        .uvs = gui::FramedSprite::LayoutConfig::UVs{
            .topLeft = {0.0f, 0.0f, 2.0f, 2.0f},
            .topRight = {4.0f, 0.0f, 2.0f, 2.0f},
            .bottomLeft = {0.0f, 4.0f, 2.0f, 2.0f},
            .bottomRight = {4.0f, 4.0f, 2.0f, 2.0f},
        }
    };
}

}  // namespace

namespace gui
{

std::unique_ptr<ListBox> ListBox::create()
{
    return std::unique_ptr<ListBox>{new ListBox{}};
}

ListBox::ListBox()
: style_{style::StyleFactory::instance().editBox}
, background_{buildLayoutConfigForListBoxTexture()}
, focusTexture_{TextureLibrary::instance().get("editbox_active")}
, normalTexture_{TextureLibrary::instance().get("editbox_inactive")}
, textStyle_{style::StyleFactory::instance().button.text}
, contentTexture_{}
, contentSprite_{}
, scrollbar_{nullptr}
, items_{}
, onSelectionChange_{}
, selectedIndex_{}
, hoveredIndex_{}
, scrollValue_{1.f}
{
    enableFocus();

    background_.setTexture(normalTexture_);
    background_.setColor(style_.backgroundColor);

    auto scrollbar = gui::scrollbar::Vertical::create();
    scrollbar_ = scrollbar.get();
    scrollbar_->setVisibility(false);
    scrollbar_->onValueChange([this](const float value) {
        scrollValue_ = value;
    });
    Widget::addChild(std::move(scrollbar));
}

void ListBox::setItems(const std::vector<Item>& items)
{
    items_ = items;
    hoveredIndex_.reset();
    syncSelection();
    updateScrollbar();
}

void ListBox::addItem(const Item& item)
{
    items_.push_back(item);
    updateScrollbar();
}

void ListBox::clear()
{
    items_.clear();
    selectedIndex_.reset();
    hoveredIndex_.reset();
    scrollValue_ = 1.f;
    notifySelectionChange();
    updateScrollbar();
}

const std::vector<ListBox::Item>& ListBox::getItems() const
{
    return items_;
}

void ListBox::setSelectedIndex(std::optional<std::size_t> index)
{
    if (index && *index >= items_.size())
    {
        index.reset();
    }

    if (selectedIndex_ == index)
    {
        return;
    }

    selectedIndex_ = index;
    scrollSelectionIntoView();
    notifySelectionChange();
}

std::optional<std::size_t> ListBox::getSelectedIndex() const
{
    return selectedIndex_;
}

void ListBox::onSelectionChange(std::function<void(std::optional<std::size_t>)> callback)
{
    onSelectionChange_ = std::move(callback);
}

void ListBox::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(background_);

    const auto viewportWidth = getViewportWidth();
    const auto viewportHeight = getViewportHeight();
    if (viewportWidth <= 0.f || viewportHeight <= 0.f)
    {
        return;
    }

    const auto textureWidth = static_cast<unsigned int>(std::ceil(viewportWidth));
    const auto textureHeight = static_cast<unsigned int>(std::ceil(viewportHeight));
    if (contentTexture_.getSize().x != textureWidth || contentTexture_.getSize().y != textureHeight)
    {
        contentTexture_.create(textureWidth, textureHeight);
        contentSprite_.setTexture(contentTexture_.getTexture(), true);
    }

    contentTexture_.clear(style_.backgroundColor);

    const auto itemHeight = getItemHeight();
    const auto firstVisibleIndex = getFirstVisibleIndex();
    const auto visibleSpan = getVisibleItemSpan();
    const auto lastVisibleIndex = std::min(items_.size(), firstVisibleIndex + visibleSpan);
    const auto viewportPosition = getGlobalPosition() + sf::Vector2f{INNER_PADDING, INNER_PADDING};

    for (std::size_t index = firstVisibleIndex; index < lastVisibleIndex; ++index)
    {
        const auto rowTop = static_cast<float>(index - firstVisibleIndex) * itemHeight;
        const auto isSelected = selectedIndex_ && *selectedIndex_ == index;
        const auto isHovered = hoveredIndex_ && *hoveredIndex_ == index;

        sf::RectangleShape row{{viewportWidth, itemHeight}};
        row.setPosition({0.f, rowTop});
        row.setFillColor(style_.backgroundColor);

        if (isSelected)
        {
            row.setFillColor({0, 0, 128, 255});
        }
        else if (isHovered)
        {
            row.setFillColor({220, 220, 220, 255});
        }

        contentTexture_.draw(row);

        sf::Text text;
        text.setFont(FontLibrary::get(textStyle_.fontName));
        text.setCharacterSize(textStyle_.size);
        text.setFillColor(isSelected ? sf::Color::White : textStyle_.color);
        text.setString(items_[index].text);
        text.setPosition({
            ROW_HORIZONTAL_PADDING,
            rowTop + ROW_VERTICAL_PADDING
        });
        contentTexture_.draw(text);
    }

    contentTexture_.display();
    contentSprite_.setPosition(viewportPosition);
    renderTexture.draw(contentSprite_);
}

void ListBox::onSizeChange()
{
    updateLayout();
}

void ListBox::onPositionChange()
{
    updateLayout();
}

EventStatus ListBox::on(const event::MouseMoved& mouseMovedEvent)
{
    hoveredIndex_ = getItemIndexAt({mouseMovedEvent.position.x, mouseMovedEvent.position.y});
    return hoveredIndex_ ? EventStatus::Consumed : EventStatus::NotConsumed;
}

EventStatus ListBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    focus();

    const auto itemIndex = getItemIndexAt({mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y});
    if (not itemIndex)
    {
        return EventStatus::Consumed;
    }

    if (selectedIndex_ != itemIndex)
    {
        selectedIndex_ = itemIndex;
        notifySelectionChange();
    }

    activateItem(*itemIndex);
    return EventStatus::Consumed;
}

EventStatus ListBox::on(const event::MouseLeft&)
{
    hoveredIndex_.reset();
    return EventStatus::NotConsumed;
}

EventStatus ListBox::on(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    if (not isFocused() || items_.empty())
    {
        return EventStatus::NotConsumed;
    }

    if (keyboardKeyReleasedEvent.key == gui::event::Key::Up)
    {
        const auto newIndex = selectedIndex_ ? std::max<std::size_t>(0, *selectedIndex_ - 1) : 0;
        setSelectedIndex(newIndex);
        return EventStatus::Consumed;
    }

    if (keyboardKeyReleasedEvent.key == gui::event::Key::Down)
    {
        const auto newIndex = selectedIndex_
            ? std::min(items_.size() - 1, *selectedIndex_ + 1)
            : 0;
        setSelectedIndex(newIndex);
        return EventStatus::Consumed;
    }

    if (keyboardKeyReleasedEvent.key == gui::event::Key::Enter && selectedIndex_)
    {
        activateItem(*selectedIndex_);
        return EventStatus::Consumed;
    }

    return EventStatus::NotConsumed;
}

EventStatus ListBox::on(const event::FocusLost&)
{
    background_.setTexture(normalTexture_);
    return EventStatus::Consumed;
}

EventStatus ListBox::on(const event::FocusGained&)
{
    background_.setTexture(focusTexture_);
    return EventStatus::Consumed;
}

void ListBox::updateLayout()
{
    background_.setPosition(getGlobalPosition());
    background_.setSize(getSize());
    updateScrollbar();
}

void ListBox::updateScrollbar()
{
    const auto viewportHeight = std::max(0.f, getSize().y - INNER_PADDING * 2.f);
    const auto itemHeight = getItemHeight();
    const auto totalContentHeight = static_cast<float>(items_.size()) * itemHeight;
    const auto canScroll = totalContentHeight > viewportHeight && viewportHeight > 0.f;

    scrollbar_->setVisibility(canScroll);

    if (canScroll)
    {
        scrollbar_->setPosition({getSize().x - INNER_PADDING - SCROLLBAR_WIDTH, INNER_PADDING});
        scrollbar_->setSize({SCROLLBAR_WIDTH, viewportHeight});
        scrollbar_->setThumbRatio(std::clamp(viewportHeight / totalContentHeight, 0.f, 1.f));

        const auto maxFirstVisibleIndex = getMaxFirstVisibleIndex();
        const auto step = maxFirstVisibleIndex > 0 ? 1.f / static_cast<float>(maxFirstVisibleIndex) : 1.f;
        scrollbar_->setStep(step);
        scrollbar_->setValue(scrollValue_);
    }
    else
    {
        scrollValue_ = 1.f;
    }
}

void ListBox::syncSelection()
{
    if (selectedIndex_ && *selectedIndex_ >= items_.size())
    {
        selectedIndex_.reset();
        notifySelectionChange();
    }
}

void ListBox::scrollSelectionIntoView()
{
    if (not selectedIndex_)
    {
        return;
    }

    const auto visibleSpan = getVisibleItemSpan();
    if (visibleSpan == 0)
    {
        return;
    }

    auto firstVisibleIndex = getFirstVisibleIndex();
    if (*selectedIndex_ < firstVisibleIndex)
    {
        firstVisibleIndex = *selectedIndex_;
    }
    else if (*selectedIndex_ >= firstVisibleIndex + visibleSpan)
    {
        firstVisibleIndex = *selectedIndex_ - visibleSpan + 1;
    }

    const auto maxFirstVisibleIndex = getMaxFirstVisibleIndex();
    if (maxFirstVisibleIndex == 0)
    {
        scrollValue_ = 1.f;
    }
    else
    {
        firstVisibleIndex = std::min(firstVisibleIndex, maxFirstVisibleIndex);
        scrollValue_ = 1.f - (static_cast<float>(firstVisibleIndex) / static_cast<float>(maxFirstVisibleIndex));
    }

    updateScrollbar();
}

float ListBox::getViewportWidth() const
{
    auto width = getSize().x - INNER_PADDING * 2.f;
    if (scrollbar_->isVisible())
    {
        width -= SCROLLBAR_WIDTH + SCROLLBAR_SPACING;
    }
    return std::max(0.f, width);
}

float ListBox::getViewportHeight() const
{
    return std::max(0.f, getSize().y - INNER_PADDING * 2.f);
}

float ListBox::getItemHeight() const
{
    return getFontHeight(FontLibrary::get(textStyle_.fontName), textStyle_.size) + ROW_VERTICAL_PADDING * 2.f;
}

std::size_t ListBox::getVisibleItemCapacity() const
{
    const auto itemHeight = getItemHeight();
    if (itemHeight <= 0.f)
    {
        return 0;
    }

    return std::max<std::size_t>(1, static_cast<std::size_t>(std::floor(getViewportHeight() / itemHeight)));
}

std::size_t ListBox::getVisibleItemSpan() const
{
    const auto itemHeight = getItemHeight();
    if (itemHeight <= 0.f)
    {
        return 0;
    }

    return std::max<std::size_t>(1, static_cast<std::size_t>(std::ceil(getViewportHeight() / itemHeight)));
}

std::size_t ListBox::getMaxFirstVisibleIndex() const
{
    const auto visibleSpan = getVisibleItemSpan();
    if (items_.size() <= visibleSpan)
    {
        return 0;
    }

    return items_.size() - visibleSpan;
}

std::size_t ListBox::getFirstVisibleIndex() const
{
    const auto maxFirstVisibleIndex = getMaxFirstVisibleIndex();
    if (maxFirstVisibleIndex == 0)
    {
        return 0;
    }

    const auto clampedScrollValue = std::clamp(scrollValue_, 0.f, 1.f);
    return static_cast<std::size_t>(std::round((1.f - clampedScrollValue) * static_cast<float>(maxFirstVisibleIndex)));
}

std::optional<std::size_t> ListBox::getItemIndexAt(const sf::Vector2f& globalPosition) const
{
    const auto viewportPosition = getGlobalPosition() + sf::Vector2f{INNER_PADDING, INNER_PADDING};
    const auto viewportWidth = getViewportWidth();
    const auto viewportHeight = getViewportHeight();

    if (globalPosition.x < viewportPosition.x || globalPosition.y < viewportPosition.y)
    {
        return std::nullopt;
    }

    if (globalPosition.x > viewportPosition.x + viewportWidth || globalPosition.y > viewportPosition.y + viewportHeight)
    {
        return std::nullopt;
    }

    const auto itemHeight = getItemHeight();
    const auto localY = globalPosition.y - viewportPosition.y;
    const auto relativeIndex = static_cast<std::size_t>(localY / itemHeight);
    const auto index = getFirstVisibleIndex() + relativeIndex;

    if (index >= items_.size())
    {
        return std::nullopt;
    }

    return index;
}

void ListBox::notifySelectionChange()
{
    if (onSelectionChange_)
    {
        onSelectionChange_(selectedIndex_);
    }
}

void ListBox::activateItem(std::size_t index)
{
    if (index < items_.size() && items_[index].onClick)
    {
        items_[index].onClick();
    }
}

}  // namespace gui
