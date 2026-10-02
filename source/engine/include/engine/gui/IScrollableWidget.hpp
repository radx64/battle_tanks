#pragma once

#include <functional>

#include "engine/Rect.hpp"
#include "engine/gui/Widget.hpp"
#include "engine/Vector2.hpp"

namespace engine::gui
{

class IScrollableWidget : public Widget
{
public:
    virtual ~IScrollableWidget() = default;

    void onViewChange(std::function<void()> callback)
    {
        onViewChangeCallback_ = std::move(callback);
    }

    void onFocusRectChange(std::function<void(const engine::FloatRect&)> callback)
    {
        onFocusRectChangeCallback_ = std::move(callback);
    }

    virtual engine::Vec2f getContentSize() = 0;
    virtual void applyOffset(const engine::Vec2f& offset) = 0;

protected:
    void notifyViewChange()
    {
        if (onViewChangeCallback_)
        {
            onViewChangeCallback_();
        }
    }

    void notifyFocusRectChange(const engine::FloatRect& position)
    {
        if (onFocusRectChangeCallback_)
        {
            onFocusRectChangeCallback_(position);
        }
    }

    std::function<void()> onViewChangeCallback_;
    std::function<void(const engine::FloatRect&)> onFocusRectChangeCallback_;
};

}  // namespace engine::gui
