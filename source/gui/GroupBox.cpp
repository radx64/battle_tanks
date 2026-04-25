#include "gui/GroupBox.hpp"

#include <algorithm>
#include <cmath>

#include "gui/Alignment.hpp"
#include "gui/style/StyleFactory.hpp"

namespace
{

const auto DEFAULT_BORDER_COLOR = sf::Color::Black;
constexpr auto DEFAULT_BORDER_THICKNESS = 1.f;
constexpr auto DEFAULT_OUTER_PADDING = 8.f;
constexpr auto DEFAULT_PADDING = 8.f;
constexpr auto DEFAULT_TITLE_PADDING = 6.f;
constexpr auto DEFAULT_TITLE_TO_FRAME_SPACING = 4.f;

}  // namespace

namespace gui
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

void GroupBox::setBorderColor(const sf::Color& color)
{
    borderColor_ = color;
}

void GroupBox::setBorderThickness(float thickness)
{
    borderThickness_ = std::max(0.f, thickness);
    updateLayout();
}

sf::FloatRect GroupBox::getContentBounds() const
{
    return contentBounds_;
}

void GroupBox::onRender(sf::RenderTexture& renderTexture)
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
        drawFrameSegment(renderTexture, {frameLeft, frameTop, std::max(0.f, titleStart - titlePadding_ - frameLeft), borderThickness_});
        drawFrameSegment(renderTexture, {titleEnd, frameTop, std::max(0.f, frameRight - titleEnd), borderThickness_});
    }
    else
    {
        drawFrameSegment(renderTexture, {frameLeft, frameTop, std::max(0.f, frameRight - frameLeft), borderThickness_});
    }

    drawFrameSegment(renderTexture, {frameLeft, frameTop, borderThickness_, std::max(0.f, frameBottom - frameTop)});
    drawFrameSegment(renderTexture, {std::max(frameLeft, frameRight - borderThickness_), frameTop, borderThickness_, std::max(0.f, frameBottom - frameTop)});
    drawFrameSegment(renderTexture, {frameLeft, std::max(frameTop, frameBottom - borderThickness_), std::max(0.f, frameRight - frameLeft), borderThickness_});

    renderTexture.draw(titleText_);
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

    titleText_.setGlobalPosition(getGlobalPosition() + sf::Vector2f{outerPadding_ + padding_ + titlePadding_, outerPadding_});
    titleText_.setSize({std::ceil(titleBounds.x), std::ceil(titleBounds.y)});
    updateChildrenLayout();
}

void GroupBox::updateChildrenLayout()
{
    const auto childPosition = sf::Vector2f{contentBounds_.left, contentBounds_.top};
    const auto childSize = sf::Vector2f{contentBounds_.width, contentBounds_.height};

    for (auto& child : children_)
    {
        child->setPosition(childPosition);
        child->setSize(childSize);
    }
}

void GroupBox::drawFrameSegment(sf::RenderTexture& renderTexture, const sf::FloatRect& rect) const
{
    if (rect.width <= 0.f || rect.height <= 0.f || borderThickness_ <= 0.f)
    {
        return;
    }

    sf::RectangleShape segment{{rect.width, rect.height}};
    segment.setPosition(getGlobalPosition() + sf::Vector2f{rect.left, rect.top});
    segment.setFillColor(borderColor_);
    renderTexture.draw(segment);
}

}  // namespace gui
