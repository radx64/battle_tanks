#include "gui/Label.hpp"

#include "gui/Debug.hpp"
#include "gui/style/StyleFactory.hpp"

namespace gui 
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

void Label::setFontColor(const sf::Color& color)
{
    text_.setFillColor(color);
}

sf::FloatRect Label::getTextBounds() const
{
    return text_.getTextBounds();
}

void Label::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(text_);

    debug::drawBox(renderTexture, getGlobalPosition(), getSize(), sf::Color::Cyan, -2.f);
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
    text_.setGlobalPosition(getGlobalPosition());
    text_.setSize(getSize());

    text_.setOffset(calculateAlignmentOffset(
        getSize(), boundsToSize(text_.getTextBounds()), alignment_));
}

void Label::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    recalculatePositionAndSize();
}

}  // namespace gui
