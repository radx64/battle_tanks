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

    debug::draw_bounds(renderTexture, this);
}

void Label::setText(const sf::String& text)
{
    text_.setString(text);
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

    // TODO: this X-height calculation is hihi hehe
    // I need to figureout something to have nice centered view
    // of fonts
    auto text_data = text_.getString();
    text_.setString("Ay");
    auto x_height = text_.getLocalBounds().height;
    auto char_size = text_.getCharacterSize();
    text_.setString(text_data);

    if (isBitSet(alignment_, gui::Alignment::Top))
    {
        y_offset -= char_size-x_height;
    }
    else if (isBitSet(alignment_, gui::Alignment::VerticallyCentered))
    {
        y_offset = getSize().y / 2 -  (text_.getCharacterSize() + (char_size-x_height)) / 2;
    }
    else if (isBitSet(alignment_, gui::Alignment::Bottom))
    {
        y_offset = getSize().y - text_.getCharacterSize();
    }

    text_.setPosition(getGlobalPosition() + sf::Vector2f{x_offset, y_offset});
}

void Label::setAlignment(const gui::Alignment& alignment)
{
    alignment_ = alignment;
    recalculatePositionAndSize();
}

}  // namespace gui