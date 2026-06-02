#pragma once

#include <algorithm>

namespace gui
{

struct ScrollPadding
{
    float left{0.f};
    float top{0.f};
    float right{0.f};
    float bottom{0.f};
};

struct ScrollAxisMetrics
{
    float effectiveContentExtent{0.f};
    float maxOffset{0.f};
    float thumbRatio{1.f};
    bool canScroll{false};
};

inline ScrollAxisMetrics calculateScrollAxisMetrics(
    const float contentExtent,
    const float viewportExtent,
    const float leadingPadding = 0.f,
    const float trailingPadding = 0.f)
{
    ScrollAxisMetrics metrics{};
    metrics.effectiveContentExtent = std::max(0.f, contentExtent + leadingPadding + trailingPadding);
    metrics.maxOffset = std::max(0.f, metrics.effectiveContentExtent - viewportExtent);
    metrics.canScroll = viewportExtent > 0.f && metrics.maxOffset > 0.f;

    if (not metrics.canScroll || metrics.effectiveContentExtent <= 0.f)
    {
        metrics.thumbRatio = 1.f;
    }
    else
    {
        metrics.thumbRatio = std::clamp(viewportExtent / metrics.effectiveContentExtent, 0.f, 1.f);
    }

    return metrics;
}

inline float calculateScrollBarValue(const float scrollOffset, const float maxScrollOffset, const bool invert)
{
    if (maxScrollOffset <= 0.f)
    {
        return invert ? 1.f : 0.f;
    }

    const auto normalizedOffset = std::clamp(scrollOffset / maxScrollOffset, 0.f, 1.f);
    return invert ? 1.f - normalizedOffset : normalizedOffset;
}

inline float calculateScrollOffset(const float scrollBarValue, const float maxScrollOffset, const bool invert)
{
    if (maxScrollOffset <= 0.f)
    {
        return 0.f;
    }

    const auto normalizedValue = std::clamp(scrollBarValue, 0.f, 1.f);
    return (invert ? (1.f - normalizedValue) : normalizedValue) * maxScrollOffset;
}

}  // namespace gui
