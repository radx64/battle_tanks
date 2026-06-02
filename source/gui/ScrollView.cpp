#include "gui/ScrollView.hpp"

#include <memory>

#include <fmt/format.h>

#include "gui/ScrollMetrics.hpp"
#include "gui/scrollbar/Horizontal.hpp"
#include "gui/scrollbar/Vertical.hpp"

namespace gui
{

namespace
{
constexpr float SCROLLBAR_THICKNESS = 32.f;
constexpr float SCROLLBAR_SPACING = 4.f;
}

std::unique_ptr<ScrollView> ScrollView::create()
{
    return std::unique_ptr<ScrollView>{new ScrollView{}};
}

void ScrollView::setContent(std::unique_ptr<IScrollableWidget> content)
{
    if (content_ != nullptr)
    {
        removeChild(content_);
        content_ = nullptr;
    }

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

    addChild(std::move(content));
    updateScrollBars();
}

void ScrollView::setScrollPadding(const ScrollPadding& scrollPadding)
{
    scrollPadding_ = scrollPadding;
    if (content_ != nullptr)
    {
        updateScrollBars();
    }
}

ScrollView::ScrollView()
{
    auto horizontalScrollBar = scrollbar::Horizontal::create();
    horizontalScrollBar_ = horizontalScrollBar.get();
    horizontalScrollBar->setValue(0.f);

    auto verticalScrollBar = scrollbar::Vertical::create();
    verticalScrollBar_ = verticalScrollBar.get();
    verticalScrollBar->setValue(1.f);

    horizontalScrollBar->onValueChange([this](const float){
        if (not isUpdatingScrollBars_)
        {
            applyScroll();
        }
    });
    verticalScrollBar->onValueChange([this](const float value){
        logger_.debug(fmt::format("Vertical scroll bar value changed, applying scroll {}", value));
        if (not isUpdatingScrollBars_)
        {
            applyScroll();
        }
    });

    horizontalScrollBar_->setThumbRatio(1.f);
    verticalScrollBar_->setThumbRatio(1.f);
    horizontalScrollBar_->setVisibility(false);
    verticalScrollBar_->setVisibility(false);

    addChild(std::move(horizontalScrollBar));
    addChild(std::move(verticalScrollBar));
}

void ScrollView::onRender(sf::RenderTexture& renderTexture)
{
    (void)renderTexture;
}

void ScrollView::onSizeChange()
{
    if (content_ != nullptr)
    {
        updateScrollBars();
    }
    else
    {
        applyLayout(false, false);
    }
}

void ScrollView::applyLayout(const bool showHorizontalScrollBar, const bool showVerticalScrollBar)
{
    const auto viewportSize = calculateViewportSize(showHorizontalScrollBar, showVerticalScrollBar);

    if (content_ != nullptr)
    {
        content_->setPosition({0.f, 0.f});
        content_->setSize(viewportSize);
    }

    horizontalScrollBar_->setVisibility(showHorizontalScrollBar);
    verticalScrollBar_->setVisibility(showVerticalScrollBar);

    horizontalScrollBar_->setPosition({0.f, viewportSize.y + SCROLLBAR_SPACING});
    horizontalScrollBar_->setSize({viewportSize.x, SCROLLBAR_THICKNESS});

    verticalScrollBar_->setPosition({viewportSize.x + SCROLLBAR_SPACING, 0.f});
    verticalScrollBar_->setSize({SCROLLBAR_THICKNESS, viewportSize.y});
}

sf::Vector2f ScrollView::calculateViewportSize(const bool showHorizontalScrollBar, const bool showVerticalScrollBar) const
{
    auto viewportSize = getSize();
    if (showVerticalScrollBar)
    {
        viewportSize.x -= SCROLLBAR_THICKNESS + SCROLLBAR_SPACING;
    }
    if (showHorizontalScrollBar)
    {
        viewportSize.y -= SCROLLBAR_THICKNESS + SCROLLBAR_SPACING;
    }

    viewportSize.x = std::max(0.f, viewportSize.x);
    viewportSize.y = std::max(0.f, viewportSize.y);
    return viewportSize;
}

void ScrollView::applyScroll()
{
    if (content_ == nullptr)
    {
        return;
    }

    const auto contentSize = content_->getContentSize();
    const auto viewportSize = content_->getSize();
    const auto horizontalMetrics = calculateScrollAxisMetrics(
        contentSize.x,
        viewportSize.x,
        scrollPadding_.left,
        scrollPadding_.right);
    const auto verticalMetrics = calculateScrollAxisMetrics(
        contentSize.y,
        viewportSize.y,
        scrollPadding_.top,
        scrollPadding_.bottom);

    scrollOffset_.x = calculateScrollOffset(horizontalScrollBar_->getValue(), horizontalMetrics.maxOffset, false);
    scrollOffset_.y = calculateScrollOffset(verticalScrollBar_->getValue(), verticalMetrics.maxOffset, true);

    const sf::Vector2f contentOffset{
        scrollPadding_.left - scrollOffset_.x,
        scrollPadding_.top - scrollOffset_.y
    };
    content_->applyOffset(contentOffset);
}

void ScrollView::updateScrollBars()
{
    if (content_ == nullptr || isUpdatingScrollBars_)
    {
        return;
    }

    isUpdatingScrollBars_ = true;

    bool showHorizontalScrollBar = false;
    bool showVerticalScrollBar = false;

    for (int iteration = 0; iteration < 3; ++iteration)
    {
        applyLayout(showHorizontalScrollBar, showVerticalScrollBar);

        const auto contentSize = content_->getContentSize();
        const auto viewportSize = content_->getSize();
        const auto horizontalMetrics = calculateScrollAxisMetrics(
            contentSize.x,
            viewportSize.x,
            scrollPadding_.left,
            scrollPadding_.right);
        const auto verticalMetrics = calculateScrollAxisMetrics(
            contentSize.y,
            viewportSize.y,
            scrollPadding_.top,
            scrollPadding_.bottom);

        if (horizontalMetrics.canScroll == showHorizontalScrollBar &&
            verticalMetrics.canScroll == showVerticalScrollBar)
        {
            break;
        }

        showHorizontalScrollBar = horizontalMetrics.canScroll;
        showVerticalScrollBar = verticalMetrics.canScroll;
    }

    applyLayout(showHorizontalScrollBar, showVerticalScrollBar);

    const auto contentSize = content_->getContentSize();
    const auto viewportSize = content_->getSize();
    const auto horizontalMetrics = calculateScrollAxisMetrics(
        contentSize.x,
        viewportSize.x,
        scrollPadding_.left,
        scrollPadding_.right);
    const auto verticalMetrics = calculateScrollAxisMetrics(
        contentSize.y,
        viewportSize.y,
        scrollPadding_.top,
        scrollPadding_.bottom);

    scrollOffset_.x = std::clamp(scrollOffset_.x, 0.f, horizontalMetrics.maxOffset);
    scrollOffset_.y = std::clamp(scrollOffset_.y, 0.f, verticalMetrics.maxOffset);

    logger_.debug(fmt::format(
        "updateScrollBars: horizontalRatio={}, verticalRatio={}",
        horizontalMetrics.thumbRatio,
        verticalMetrics.thumbRatio));

    horizontalScrollBar_->setThumbRatio(horizontalMetrics.thumbRatio);
    verticalScrollBar_->setThumbRatio(verticalMetrics.thumbRatio);
    horizontalScrollBar_->setValue(calculateScrollBarValue(scrollOffset_.x, horizontalMetrics.maxOffset, false));
    verticalScrollBar_->setValue(calculateScrollBarValue(scrollOffset_.y, verticalMetrics.maxOffset, true));

    isUpdatingScrollBars_ = false;
    applyScroll();
}

void ScrollView::ensureRectVisible(const sf::FloatRect& rectBounds)
{
    if (content_ == nullptr)
    {
        return;
    }

    const auto contentSize = content_->getContentSize();
    const auto viewportSize = content_->getSize();
    const auto horizontalMetrics = calculateScrollAxisMetrics(
        contentSize.x,
        viewportSize.x,
        scrollPadding_.left,
        scrollPadding_.right);
    const auto verticalMetrics = calculateScrollAxisMetrics(
        contentSize.y,
        viewportSize.y,
        scrollPadding_.top,
        scrollPadding_.bottom);

    const sf::FloatRect paddedRectBounds{
        rectBounds.left + scrollPadding_.left,
        rectBounds.top + scrollPadding_.top,
        rectBounds.width,
        rectBounds.height
    };

    sf::Vector2f scrollOffset = scrollOffset_;
    if (paddedRectBounds.left < scrollOffset.x)
    {
        scrollOffset.x = paddedRectBounds.left;
    }
    if (paddedRectBounds.left + paddedRectBounds.width > scrollOffset.x + viewportSize.x)
    {
        scrollOffset.x = paddedRectBounds.left + paddedRectBounds.width - viewportSize.x;
    }
    if (paddedRectBounds.top < scrollOffset.y)
    {
        scrollOffset.y = paddedRectBounds.top;
    }
    if (paddedRectBounds.top + paddedRectBounds.height > scrollOffset.y + viewportSize.y)
    {
        scrollOffset.y = paddedRectBounds.top + paddedRectBounds.height - viewportSize.y;
    }

    scrollOffset.x = std::clamp(scrollOffset.x, 0.f, horizontalMetrics.maxOffset);
    scrollOffset.y = std::clamp(scrollOffset.y, 0.f, verticalMetrics.maxOffset);

    isUpdatingScrollBars_ = true;
    horizontalScrollBar_->setValue(calculateScrollBarValue(scrollOffset.x, horizontalMetrics.maxOffset, false));
    verticalScrollBar_->setValue(calculateScrollBarValue(scrollOffset.y, verticalMetrics.maxOffset, true));
    isUpdatingScrollBars_ = false;

    applyScroll();
}

}  // namespace gui
