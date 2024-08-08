#include "gui/Label.hpp"

#include "gui/StyleSheet.hpp"

#include "gui/Debug.hpp"

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
    text_.setPosition(Component::getGlobalPosition());
}

void Label::onRender(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(text_);
}

void Label::setText(const std::string_view& text)
{
    text_.setString(text.data());
    bounds_.width = text_.getLocalBounds().width;
    bounds_.height = text_.getLocalBounds().height;
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
    float x_offset{};
    float y_offset{};

    if (isBitSet(alignment_, gui::Alignment::Left))
    {
        x_offset = 0;
    }
    else if (isBitSet(alignment_, gui::Alignment::HorizontallyCentered))
    {
        x_offset = getSize().x / 2 -  text_.getLocalBounds().width / 2;
    }
    else if (isBitSet(alignment_, gui::Alignment::Right))
    {
        x_offset = getSize().x - text_.getLocalBounds().width;
    }

    // FIXME: font height is problematic to calculate as SFML is keeping common baseline
    // so top of a label has an empty space
    // there are few possible solution for vertically centering this
    // I need to work on that later

    if (isBitSet(alignment_, gui::Alignment::Top))
    {
        y_offset -= 0;
    }
    else if (isBitSet(alignment_, gui::Alignment::VerticallyCentered))
    {
        y_offset = getSize().y / 2 - text_.getLocalBounds().height / 2;

    }
    else if (isBitSet(alignment_, gui::Alignment::Bottom))
    {
        y_offset = getSize().y - text_.getLocalBounds().height;
    }

    text_.setPosition(getGlobalPosition() + sf::Vector2f{x_offset, y_offset});
}

void Label::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    recalculatePositionAndSize();
}

}  // namespace gui