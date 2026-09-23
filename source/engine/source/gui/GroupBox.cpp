#include "engine/gui/GroupBox.hpp"

#include <algorithm>
#include <cmath>

#include "engine/gfx/Color.hpp"
#include "engine/gui/Alignment.hpp"
#include "engine/gui/style/StyleFactory.hpp"

namespace
{

const auto DEFAULT_BORDER_COLOR = engine::gfx::Color::Black;
constexpr auto DEFAULT_BORDER_THICKNESS = 1.f;
constexpr auto DEFAULT_OUTER_PADDING = 8.f;
constexpr auto DEFAULT_PADDING = 8.f;
constexpr auto DEFAULT_TITLE_PADDING = 6.f;
constexpr auto DEFAULT_TITLE_TO_FRAME_SPACING = 4.f;

}  // namespace

namespace engine::gui
{

std::unique_ptr<GroupBox> GroupBox::create(const std::string_view& title)
{
    return std::unique_ptr<GroupBox>{new GroupBox{title}};
}

GroupBox::GroupBox(const std::string_view& title)
: titleText_{style::StyleFactory::instance().label}
, borderColor_{DEFAULT_BORDER_COLOR}
, borderThickness_{DEFAULT_BORDER_THICKNESS}
, outerPadding_{DEFAULT_OUTER_PADDING}
, padding_{DEFAULT_PADDING}
, titlePadding_{DEFAULT_TITLE_PADDING}
, titleToFrameSpacing_{DEFAULT_TITLE_TO_FRAME_SPACING}
, contentBounds_{}
{
    setTitle(title);
}

void GroupBox::addChild(std::unique_ptr<Widget> child)
{
    child->setPosition({contentBounds_.left, contentBounds_.top});
    child->setSize({contentBounds_.width, contentBounds_.height});
    Widget::addChild(std::move(child));
}

void GroupBox::setTitle(const std::string_view& title)
{
    titleText_.setText(title);
    updateLayout();
}

std::string GroupBox::getTitle() const
{
    return titleText_.getText();
}

void GroupBox::setPadding(float padding)
{
    padding_ = std::max(0.f, padding);
    updateLayout();
}

float GroupBox::getPadding() const
{
    return padding_;
}

void GroupBox::setOuterPadding(float padding)
{
    outerPadding_ = std::max(0.f, padding);
    updateLayout();
}

float GroupBox::getOuterPadding() const
{
    return outerPadding_;
}

void GroupBox::setBorderColor(const engine::gfx::Color& color)
{
    borderColor_ = color;
}

void GroupBox::setBorderThickness(float thickness)
{
    borderThickness_ = std::max(0.f, thickness);
    updateLayout();
}

engine::FloatRect GroupBox::getContentBounds() const
{
    return contentBounds_;
}

void GroupBox::onRender(engine::gfx::RenderTarget& renderTarget)
{
    const auto size = getSize();
    if (size.x <= 0.f || size.y <= 0.f)
    {
        return;
    }

    const auto titleBounds = boundsToSize(titleText_.getTextBounds());
    const auto frameLeft = std::min(size.x, outerPadding_);
    const auto frameRight = std::max(frameLeft, size.x - outerPadding_);
    const auto frameTop = std::min(size.y, outerPadding_ + std::ceil(titleBounds.y / 2.f));
    const auto frameBottom = std::max(frameTop, size.y - outerPadding_);
    const auto titleStart = outerPadding_ + padding_ + titlePadding_;
    const auto titleEnd = std::min(frameRight, titleStart + titleBounds.x + titlePadding_ * 2.f);

    if (titleBounds.x > 0.f)
    {
        drawFrameSegment(renderTarget, {frameLeft, frameTop, std::max(0.f, titleStart - titlePadding_ - frameLeft), borderThickness_});
        drawFrameSegment(renderTarget, {titleEnd, frameTop, std::max(0.f, frameRight - titleEnd), borderThickness_});
    }
    else
    {
        drawFrameSegment(renderTarget, {frameLeft, frameTop, std::max(0.f, frameRight - frameLeft), borderThickness_});
    }

    drawFrameSegment(renderTarget, {frameLeft, frameTop, borderThickness_, std::max(0.f, frameBottom - frameTop)});
    drawFrameSegment(renderTarget, {std::max(frameLeft, frameRight - borderThickness_), frameTop, borderThickness_, std::max(0.f, frameBottom - frameTop)});
    drawFrameSegment(renderTarget, {frameLeft, std::max(frameTop, frameBottom - borderThickness_), std::max(0.f, frameRight - frameLeft), borderThickness_});


    renderTarget.draw(titleText_);
}

void GroupBox::onSizeChange()
{
    updateLayout();
}

void GroupBox::onPositionChange()
{
    updateLayout();
}

void GroupBox::updateLayout()
{
    const auto size = getSize();
    const auto titleBounds = boundsToSize(titleText_.getTextBounds());
    const auto frameTop = outerPadding_ + std::ceil(titleBounds.y / 2.f);
    const auto contentTop = std::max(outerPadding_ + titleBounds.y + titleToFrameSpacing_, frameTop + padding_);

    contentBounds_.left = outerPadding_ + padding_;
    contentBounds_.top = contentTop;
    contentBounds_.width = std::max(0.f, size.x - outerPadding_ * 2.f - padding_ * 2.f);
    contentBounds_.height = std::max(0.f, size.y - contentTop - outerPadding_ - padding_);

    const auto titlePosition = getGlobalPosition() + engine::Vec2f{outerPadding_ + padding_ + titlePadding_, outerPadding_};

    titleText_.setGlobalPosition(titlePosition);
    titleText_.setSize({std::ceil(titleBounds.x), std::ceil(titleBounds.y)});
    updateChildrenLayout();
}

void GroupBox::updateChildrenLayout()
{
    const auto childPosition = engine::Vec2f{contentBounds_.left, contentBounds_.top};
    const auto childSize = engine::Vec2f{contentBounds_.width, contentBounds_.height};

    for (auto& child : children_)
    {
        child->setPosition(childPosition);
        child->setSize(childSize);
    }
}

void GroupBox::drawFrameSegment(engine::gfx::RenderTarget& renderTarget, const engine::FloatRect& rect) const
{
    if (rect.width <= 0.f || rect.height <= 0.f || borderThickness_ <= 0.f)
    {
        return;
    }

    engine::gfx::RectangleShape segment{{rect.width, rect.height}};

    const auto position = getGlobalPosition() + engine::Vec2f{rect.left, rect.top};

    segment.setPosition(position);
    segment.setFillColor(borderColor_);
    renderTarget.draw(segment);
}

}  // namespace engine::gui
