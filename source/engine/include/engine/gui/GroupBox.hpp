#pragma once

#include <memory>
#include <string_view>


#include "engine/gfx/Color.hpp"
#include "engine/gui/Text.hpp"
#include "engine/gui/Widget.hpp"
#include "engine/Rect.hpp"

namespace engine::gui
{

class GroupBox : public Widget
{
public:
    static std::unique_ptr<GroupBox> create(const std::string_view& title);

    explicit GroupBox(const std::string_view& title);

    void addChild(std::unique_ptr<Widget> child) override;

    void setTitle(const std::string_view& title);
    std::string getTitle() const;

    void setPadding(float padding);
    float getPadding() const;
    void setOuterPadding(float padding);
    float getOuterPadding() const;

    void setBorderColor(const engine::gfx::Color& color);
    void setBorderThickness(float thickness);

    engine::FloatRect getContentBounds() const;

protected:
    void onRender(engine::gfx::RenderTarget& renderTarget) override;
    void onSizeChange() override;
    void onPositionChange() override;

private:
    void updateLayout();
    void updateChildrenLayout();
    void drawFrameSegment(engine::gfx::RenderTarget& renderTarget, const engine::FloatRect& rect) const;

    Text titleText_;
    engine::gfx::Color borderColor_;
    float borderThickness_;
    float outerPadding_;
    float padding_;
    float titlePadding_;
    float titleToFrameSpacing_;
    engine::FloatRect contentBounds_;
};

}  // namespace engine::gui
