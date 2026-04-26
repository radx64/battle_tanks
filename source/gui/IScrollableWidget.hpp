#pragma once

#include <functional>

#include "gui/Widget.hpp"

namespace gui
{

class IScrollableWidget : public Widget
{
public:
    virtual ~IScrollableWidget() = default;

    void onViewChange(std::function<void()> callback)
    {
        onViewChangeCallback_ = std::move(callback);
    }

    void onFocusRectChange(std::function<void(const sf::FloatRect&)> callback)
    {
        onFocusRectChangeCallback_ = std::move(callback);
    }

    virtual sf::Vector2f getViewportSize() = 0;
    virtual sf::Vector2f getContentSize() = 0;
    virtual void applyOffset(const sf::Vector2f& offset) = 0;

protected:
    void notifyViewChange()
    {
        if (onViewChangeCallback_)
        {
            onViewChangeCallback_();
        }
    }

    void notifyFocusRectChange(const sf::FloatRect& position)
    {
        if (onFocusRectChangeCallback_)
        {
            onFocusRectChangeCallback_(position);
        }
    }

    std::function<void()> onViewChangeCallback_;
    std::function<void(const sf::FloatRect&)> onFocusRectChangeCallback_;
};

}  // namespace gui
