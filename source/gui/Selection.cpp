#include "gui/Selection.hpp"

#include <cassert>

#include "gui/Text.hpp"

#include "gui/Alignment.hpp"

constexpr float CURSOR_EXTRA_HEIGHT = 6.f;

namespace gui
{

Selection::Selection(gui::Text& text)
: text_{text}
, selectionStartIndex_{}
, selectionEndIndex_{}
, selectionStartPosition_{}
, selectionEndPosition_{}
{
    //TODO move this color selection to stylesheet
    selection_.setFillColor(sf::Color(100,100,230,127));
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
    update();
}

size_t Selection::startsAt() const
{
    return selectionStartIndex_ < selectionEndIndex_ ? selectionStartIndex_ : selectionEndIndex_;
}

size_t Selection::endsAt() const
{
    return selectionStartIndex_ < selectionEndIndex_ ? selectionEndIndex_ : selectionStartIndex_;
}

size_t Selection::length() const
{
    return endsAt() - startsAt();
}

void Selection::clear()
{
    selectionEndIndex_ = selectionStartIndex_;
    selectionEndPosition_ = selectionStartPosition_;
    update();
}

void Selection::to(const uint32_t index, const sf::Vector2f& position)
{
    selectionEndIndex_ = index;
    selectionEndPosition_ = position;
    update();
}

void Selection::update()
{
    const auto textBounds = text_.getTextBounds();
    selection_.setPosition(selectionStartPosition_ + sf::Vector2f{textBounds.left, textBounds.top - CURSOR_EXTRA_HEIGHT / 2.f});
    selection_.setSize(sf::Vector2f(selectionEndPosition_.x - selectionStartPosition_.x, text_.getTextSingleLineHeight() + CURSOR_EXTRA_HEIGHT)); 
    text_.updateTexture();
}

void Selection::render(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(selection_);
}

}  // namespace gui
