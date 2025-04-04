#include "gui/Label.hpp"

#include "gui/Debug.hpp"
#include "gui/StyleSheet.hpp"

namespace gui 
{

 std::unique_ptr<Label> Label::create(const std::string_view& text)
{
    return std::unique_ptr<Label>{new Label{text}};
}

Label::Label(const std::string_view& text)
: alignment_(gui::Alignment::Left)
{
    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    //text_.setOutlineColor(style.getOutlineColor());
    //text_.setOutlineThickness(1);
    setText(text.data());
    setPosition({0.0f, 0.0f});
    setSize({20.f, 20.f});
    recalculatePositionAndSize();
}

void Label::setFontColor(const sf::Color& color)
{
    text_.setFillColor(color);
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
