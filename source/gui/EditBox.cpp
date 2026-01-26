#include "gui/EditBox.hpp"

namespace 
{
const sf::Vector2f EXTRA_END_OFFSET{6.f, 4.f};

}  // namespace

namespace gui
{

std::unique_ptr<EditBox> EditBox::create()
{
    return std::unique_ptr<EditBox>{new EditBox{}};
}

EditBox::EditBox()
: BaseEditBox()
{
}

void EditBox::updateTextVisbleArea()
{
    float textXoffset = text_.getSize().x - text_.getTextBounds().width;

    sf::Vector2f textBounds = boundsToSize(text_.getTextBounds());

    
    auto alignmentOffset = calculateAlignmentOffset(text_.getSize() - EXTRA_END_OFFSET, textBounds, alignment_);

    // if text does no fit (alignment offset is negative)
    // ignore it and behave as normal left alignment so cursor
    // will be on the right side of the text and stil visible

    if (alignmentOffset.x < 0) alignmentOffset.x = 0;

    if (textXoffset < 0)
    {
        text_.setOffset(sf::Vector2f{textXoffset, 0.f} + alignmentOffset);
    }
    else
    {
        text_.setOffset(alignmentOffset);
    }
}

EventStatus EditBox::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (not isInside(mouseButtonPressedEvent.position))
    {
        return EventStatus::NotConsumed;
    }

    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left)
    {
        return EventStatus::NotConsumed;
    }

    mouseLeftButtonPressed_ = true;

    focus();
    enterEdit();

    textCursor_.moveTo(mouseButtonPressedEvent.position.x);

    if (anyShiftHeldDown_)
    {
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }
    else
    {
        if (not selection_.isEmpty())
        {
            selection_.clear();
            text_.updateTexture();
        }
        selection_.start(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::MouseMoved& mouseMovedEvent)
{
    if(mouseLeftButtonPressed_)
    {
        textCursor_.moveTo(mouseMovedEvent.position.x);
        selection_.to(textCursor_.getIndex(), textCursor_.getPosition());
    }

    return EventStatus::NotConsumed;
}

EventStatus EditBox::on(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    // Handle Return key specific to single-line edit box
    if (keyboardKeyPressed.key == sf::Keyboard::Return)
    {
        defocusWithAllChildren();
        return EventStatus::Consumed;
    }

    // Delegate to base class for other keys
    return BaseEditBox::on(keyboardKeyPressed);
}

}  // namespace gui
