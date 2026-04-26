#pragma once

#include "gui/IScrollableWidget.hpp"
#include "gui/Widget.hpp"

namespace gui::layout { class Grid; }
namespace gui::scrollbar { class Horizontal; }
namespace gui::scrollbar { class Vertical; }

namespace gui
{

class ScrollView : public gui::Widget
{
public:
    static std::unique_ptr<ScrollView> create();
    ~ScrollView() = default;

    void setContent(std::unique_ptr<IScrollableWidget> content);
    void updateScrollBars();

protected:
    ScrollView();
    void onRender(sf::RenderTexture& renderTexture) override;
    void onSizeChange() override;
    void applyScroll();
    void ensureRectVisible(const sf::FloatRect& rectBounds);

    IScrollableWidget* content_{nullptr};
    layout::Grid* layout_{nullptr};
    scrollbar::Horizontal* horizontalScrollBar_{nullptr};
    scrollbar::Vertical* verticalScrollBar_{nullptr};
    sf::Vector2f scrollOffset_{0.f, 0.f};

    bool isUpdatingScrollBars_{false};
};

}  // namespace gui
