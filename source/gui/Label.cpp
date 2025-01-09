#include "gui/Label.hpp"

#include "gui/Debug.hpp"
#include "gui/StyleSheet.hpp"

namespace gui 
{

Label::Label(const std::string_view& text)
: alignment_(gui::Alignment::Left)
{
    auto style = BasicStyleSheetFactory::instance();
    text_.setFont(style.getFont());
    text_.setCharacterSize(style.getFontSize());
    text_.setFillColor(style.getFontColor());
    text_.setOutlineColor(style.getOutlineColor());
    text_.setOutlineThickness(style.getOutlineThickness());
    setText(text.data());
    setPosition({0.0f, 0.0f});
    text_.setSize(getSize());
    text_.setGlobalPosition(Component::getGlobalPosition());
}

void Label::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(text_);
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
    // FIXME: font height is problematic to calculate as SFML is keeping common baseline
    // so top of a label has an empty space
    // there are few possible solution for vertically centering this
    // I need to work on that later
    text_.setSize(getSize());
    text_.setGlobalPosition(getGlobalPosition());
    
    text_.setOffset(calculateAlignmentOffset(
        getSize(), boundsToSize(text_.getLocalBounds()), alignment_));
}

void Label::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    recalculatePositionAndSize();
}

}  // namespace gui
