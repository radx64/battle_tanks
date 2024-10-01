#include "gui/Selection.hpp"

#include <cassert>

#include "gui/Text.hpp"

namespace gui
{

Selection::Selection(gui::Text& text)
: text_{text}
, selectionStartIndex_{}
, selectionEndIndex_{}
, isActive_{false}
, selectionStartPosition_{}
, selectionEndPosition_{}
{
    selection_.setFillColor(sf::Color(230,100,100,127));
}

bool Selection::isActive() const
{
    return isActive_;
}

bool Selection::isEmpty() const
{
    return selectionStartIndex_ == selectionEndIndex_;
}

void Selection::start(const uint32_t index, const sf::Vector2f& position)
{
    selectionStartIndex_ = index;
    selectionStartPosition_ = position;

    selectionEndIndex_ = index;
    selectionEndPosition_ = position;

    assert(isActive_ == false && "Selection is already started, can't move start point!"); 
    isActive_ = true;
}

size_t Selection::startsAt() const
{
    return selectionStartIndex_;
}

size_t Selection::endsAt() const
{
    return selectionEndIndex_;
}

size_t Selection::length() const
{
    return endsAt() - startsAt();
}

void Selection::clear()
{
    selectionEndIndex_ = selectionStartIndex_;
    selectionEndPosition_ = selectionStartPosition_;
    end();
    update();
}

void Selection::to(const uint32_t index, const sf::Vector2f& position)
{
    selectionEndIndex_ = index;
    selectionEndPosition_ = position;
}   

void Selection::end()
{
    isActive_ = false;

    if (selectionStartIndex_ > selectionEndIndex_)
    {
        std::swap(selectionStartIndex_, selectionEndIndex_);
        std::swap(selectionStartPosition_, selectionEndPosition_);
    }
}

void Selection::update()
{
    selection_.setPosition(selectionStartPosition_);
    selection_.setSize(sf::Vector2f(selectionEndPosition_.x - selectionStartPosition_.x, text_.getTextHeight())); 
    text_.updateTexture();
}

void Selection::render(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(selection_);
}

}  // namespace gui
