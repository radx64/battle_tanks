#include "engine/gui/Label.hpp"

#include "engine/gui/Debug.hpp"
#include "engine/gui/style/StyleFactory.hpp"

namespace engine::gui 
{

std::unique_ptr<Label> Label::create(const std::string_view& text)
{
    return std::unique_ptr<Label>{new Label{text}};
}

std::unique_ptr<Label> Label::create(const std::string_view& text, const style::Text& style)
{
    return std::unique_ptr<Label>{new Label{text, style}};
}

Label::Label(const std::string_view& text)
: Label{text, style::StyleFactory::instance().label}
{
}

Label::Label(const std::string_view& text, const style::Text& style)
: alignment_{gui::Alignment::Left}
, text_{style}
{
    setText(text.data());
    setPosition({0.0f, 0.0f});
    setSize({20.f, 20.f});
    recalculatePositionAndSize();
}

void Label::setFontColor(const engine::gfx::Color& color)
{
    text_.setFillColor(color);
}

engine::FloatRect Label::getTextBounds() const
{
    return text_.getTextBounds();
}

void Label::onRender(engine::gfx::RenderTarget& renderTarget)
{
    renderTarget.draw(text_);

    debug::drawBox(renderTarget, getGlobalPosition(), getSize(), engine::gfx::Color::Cyan, -2.f);
}

void Label::setText(const std::string_view& text)
{
    text_.setText(text);
    recalculatePositionAndSize();
}

void Label::onSizeChange()
{
    recalculatePositionAndSize();
}

void Label::onPositionChange()
{
    recalculatePositionAndSize();
}

void Label::recalculatePositionAndSize()
{
    const auto position = getGlobalPosition();
    const auto size = getSize();
    text_.setGlobalPosition(position);
    text_.setSize(size);

    text_.setOffset(calculateAlignmentOffset(size, boundsToSize(text_.getTextBounds()), alignment_));
}

void Label::setAlignment(const engine::gui::Alignment& alignment)
{
    alignment_ = alignment;
    recalculatePositionAndSize();
}

}  // namespace engine::gui
