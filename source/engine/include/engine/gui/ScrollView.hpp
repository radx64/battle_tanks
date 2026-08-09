#pragma once

#include "engine/gui/ScrollMetrics.hpp"
#include "engine/gui/IScrollableWidget.hpp"
#include "engine/gui/Widget.hpp"

namespace engine::gui::scrollbar { class Horizontal; }
namespace engine::gui::scrollbar { class Vertical; }

namespace engine::gui
{

class ScrollView : public engine::gui::Widget
{
public:
    static std::unique_ptr<ScrollView> create();
    ~ScrollView() = default;

    void setContent(std::unique_ptr<IScrollableWidget> content);
    void setScrollPadding(const ScrollPadding& scrollPadding);
    void updateScrollBars();

protected:
    ScrollView();
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void applyScroll();
    void ensureRectVisible(const sf::FloatRect& rectBounds);
    void applyLayout(const bool showHorizontalScrollBar, const bool showVerticalScrollBar);
    sf::Vector2f calculateViewportSize(const bool showHorizontalScrollBar, const bool showVerticalScrollBar) const;

    IScrollableWidget* content_{nullptr};
    scrollbar::Horizontal* horizontalScrollBar_{nullptr};
    scrollbar::Vertical* verticalScrollBar_{nullptr};
    sf::Vector2f scrollOffset_{0.f, 0.f};
    ScrollPadding scrollPadding_{};

    bool isUpdatingScrollBars_{false};
};

}  // namespace engine::gui
