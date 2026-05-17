#include "gui/ScrollView.hpp"

#include <memory>

#include <fmt/format.h>

#include "gui/layout/Grid.hpp"
#include "gui/scrollbar/Horizontal.hpp"
#include "gui/scrollbar/Vertical.hpp"

namespace gui
{

namespace
{
constexpr float EXTRA_VERTICAL_SCROLL_PADDING = 200.f;

float calculateMaxScroll(const float contentExtent, const float viewportExtent)
{
    return std::max(0.f, contentExtent - viewportExtent);
}

float calculateScrollBarValue(const float scrollOffset, const float maxScroll, const bool invert)
{
    if (maxScroll <= 0.f)
    {
        return invert ? 1.f : 0.f;
    }

    const auto normalizedOffset = scrollOffset / maxScroll;
    return invert ? 1.f - normalizedOffset : normalizedOffset;
}
}

// TODO: Scrollbars ranges are not updated when bounds of the text are changing

std::unique_ptr<ScrollView> ScrollView::create()
{
    return std::unique_ptr<ScrollView>{new ScrollView{}};   
}

void ScrollView::setContent(std::unique_ptr<IScrollableWidget> content)
{
    content_ = content.get();

    content_->onViewChange([this]()
    {
        logger_.debug("Content view changed, updating scroll bars and applying scroll");
        updateScrollBars();
    });

    content_->onFocusRectChange([this](const sf::FloatRect& position)
    {
        logger_.debug("Content focus rect changed, applying scroll to ensure focus rect is visible");
        ensureRectVisible(position);
    });

    layout_->setElementAt(0, 0, std::move(content));

    layout_->setSize(getSize());
    updateScrollBars();
}

ScrollView::ScrollView()
{
    auto layout = layout::Grid::create(2, 2);
    layout->setVerticalPadding(4);
    layout->setHorizontalPadding(4);

    layout->setColumnSize(1, layout::Constraint::Pixels(32));
    layout->setRowSize(1, layout::Constraint::Pixels(32));

    auto horizontalScrollBar = scrollbar::Horizontal::create();
    horizontalScrollBar_ = horizontalScrollBar.get();
    horizontalScrollBar->setValue(0.f);

    auto verticalScrollBar = scrollbar::Vertical::create();
    verticalScrollBar_ = verticalScrollBar.get();
    verticalScrollBar->setValue(1.f);

    horizontalScrollBar->onValueChange([this](const float){
        applyScroll();
    });
    verticalScrollBar->onValueChange([this](const float value){
        logger_.debug(fmt::format("Vertical scroll bar value changed, applying scroll {}", value));
        applyScroll();
    });

    horizontalScrollBar_->setThumbRatio(0.1f);
    verticalScrollBar_->setThumbRatio(0.1f);

    layout->setElementAt(0, 1, std::move(horizontalScrollBar));
    layout->setElementAt(1, 0, std::move(verticalScrollBar));

    layout_ = layout.get();
    layout_->setSize(getSize());
    addChild(std::move(layout));
}

void ScrollView::onRender(sf::RenderTexture& renderTexture)
{
    (void)renderTexture;
}

void ScrollView::onSizeChange()
{
    layout_->setSize(getSize());

    if (content_ != nullptr)
    {
        updateScrollBars();
    }
}

void ScrollView::applyScroll()
{
    if (content_ == nullptr)
    {
        return;
    }

    const auto contentSize = content_->getContentSize();
    const auto viewableSize = content_->getViewportSize();
    const auto effectiveContentSize = sf::Vector2f{contentSize.x, contentSize.y + EXTRA_VERTICAL_SCROLL_PADDING};

    const auto maxScrollX = calculateMaxScroll(effectiveContentSize.x, viewableSize.x);
    const auto maxScrollY = calculateMaxScroll(effectiveContentSize.y, viewableSize.y);

    scrollOffset_.x = std::min(1.0f, horizontalScrollBar_->getValue()) * maxScrollX;
    scrollOffset_.y = std::max(0.f, (1.f - verticalScrollBar_->getValue())) * maxScrollY;

    content_->applyOffset(-scrollOffset_);
}

void ScrollView::updateScrollBars()
{
    if (content_ == nullptr)
    {
        return;
    }
    
    if (isUpdatingScrollBars_)
    {
        return;
    }
    
    const auto contentSize = content_->getContentSize();
    const auto viewableSize = content_->getViewportSize();
    const auto effectiveContentSize = sf::Vector2f{contentSize.x, contentSize.y + EXTRA_VERTICAL_SCROLL_PADDING};
    const auto canScrollHorizontally = effectiveContentSize.x > viewableSize.x;
    const auto canScrollVertically = effectiveContentSize.y > viewableSize.y;

    // Calculate ratios with proper handling of edge cases
    float horizontalRatio = 1.f;
    float verticalRatio = 1.f;
    
    if (canScrollHorizontally && effectiveContentSize.x > 0.f)
    {
        horizontalRatio = std::min(1.f, viewableSize.x / effectiveContentSize.x);
    }
    if (canScrollVertically && effectiveContentSize.y > 0.f)
    {
        verticalRatio = std::min(1.f, viewableSize.y / effectiveContentSize.y);
    }
    
    logger_.debug(fmt::format("updateScrollBars: horizontalRatio={}, verticalRatio={}", horizontalRatio, verticalRatio));

    isUpdatingScrollBars_ = true;
    horizontalScrollBar_->setVisibility(canScrollHorizontally);
    verticalScrollBar_->setVisibility(canScrollVertically);
    horizontalScrollBar_->setThumbRatio(horizontalRatio);
    verticalScrollBar_->setThumbRatio(verticalRatio);
    isUpdatingScrollBars_ = false;
}


void ScrollView::ensureRectVisible(const sf::FloatRect& rectBounds)
{
    if (content_ == nullptr)
    {
        return;
    }

    const auto contentSize = content_->getContentSize();
    const auto viewableSize = content_->getViewportSize();
    const auto effectiveContentSize = sf::Vector2f{contentSize.x, contentSize.y + EXTRA_VERTICAL_SCROLL_PADDING};

    const auto maxScrollX = calculateMaxScroll(effectiveContentSize.x, viewableSize.x);
    const auto maxScrollY = calculateMaxScroll(effectiveContentSize.y, viewableSize.y);

    // Calculate scroll offset to make the rectangle visible
    sf::Vector2f scrollOffset = scrollOffset_;
    if (rectBounds.left < scrollOffset.x)
    {
        scrollOffset.x = rectBounds.left;
    }
    if (rectBounds.left + rectBounds.width > scrollOffset.x + viewableSize.x)
    {
        scrollOffset.x = rectBounds.left + rectBounds.width - viewableSize.x;
    }
    if (rectBounds.top < scrollOffset.y)
    {
        scrollOffset.y = rectBounds.top;
    }
    if (rectBounds.top + rectBounds.height > scrollOffset.y + viewableSize.y)
    {
        scrollOffset.y = rectBounds.top + rectBounds.height - viewableSize.y;
    }

    // Clamp the scroll offset to valid range
    scrollOffset.x = std::clamp(scrollOffset.x, 0.f, maxScrollX);
    scrollOffset.y = std::clamp(scrollOffset.y, 0.f, maxScrollY);

    // Update the scrollbar values
    horizontalScrollBar_->setValue(calculateScrollBarValue(scrollOffset.x, maxScrollX, false));
    verticalScrollBar_->setValue(calculateScrollBarValue(scrollOffset.y, maxScrollY, true));
}

}  // namespace gui
