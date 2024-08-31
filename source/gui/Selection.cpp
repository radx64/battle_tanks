#include "gui/Selection.hpp"

#include <cassert>

#include "gui/Text.hpp"

namespace gui
{

Selection::Selection(gui::Text& text)
: text_{text}
, selection_start_index_{}
, selection_end_index_{}
, is_ongoing_{false}
, selection_start_position_{}
, selection_end_position_{}
{
    selection_.setFillColor(sf::Color(230,100,100,127));
}

bool Selection::isOngoing() const
{
    return is_ongoing_;
}

bool Selection::isEmpty() const
{
    return selection_start_index_ == selection_end_index_;
}

void Selection::start(const uint32_t index, const sf::Vector2f& position)
{
    assert(not is_ongoing_ && "Selection can be started only once");
    is_ongoing_ = true;
    selection_start_index_ = index;
    selection_start_position_ = position;
    selection_end_index_ = selection_start_index_;
    selection_end_position_ = selection_start_position_;
}

size_t Selection::startsAt() const
{
    return selection_start_index_;
}

size_t Selection::endsAt() const
{
    return selection_end_index_;
}

size_t Selection::length() const
{
    return endsAt() - startsAt();
}

void Selection::clear()
{
    selection_end_index_ = selection_start_index_;
    selection_end_position_ = selection_start_position_;
    end();
    update();
}

void Selection::updateEnd(const uint32_t index, const sf::Vector2f& position)
{
    selection_end_index_ = index;
    selection_end_position_ = position;
}   

void Selection::end()
{
    is_ongoing_ = false;

    if (selection_start_index_ > selection_end_index_)
    {
        std::swap(selection_start_index_, selection_end_index_);
        std::swap(selection_start_position_, selection_end_position_);
    }
}

void Selection::update()
{
    selection_.setPosition(selection_start_position_);
    selection_.setSize(sf::Vector2f(selection_end_position_.x - selection_start_position_.x, text_.getTextHeight())); 
    text_.updateTexture();
}

void Selection::render(sf::RenderTexture& renderTexture)
{
    renderTexture.draw(selection_);
}

}  // namespace gui
