#include "gui/Component.hpp"

namespace gui
{

Component::Component()
: global_position_{}
, relative_position_{}
, alignment_ {}
, parent_{nullptr}
, children_ {}
, is_visible_ {true}
{
}

void Component::render(sf::RenderWindow& renderWindow)
{
    if (!is_visible_) return;
    onRender(renderWindow);
    for (auto& child : children_)
    {
        child->render(renderWindow); 
    }
}

bool Component::update(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    bool was_mouse_event_processed {false};
    was_mouse_event_processed = onMouseUpdate(mousePosition, isLeftClicked);
    for (auto& child : children_)
    {
        if (child->update(mousePosition, isLeftClicked)) was_mouse_event_processed = true;
    }
    return was_mouse_event_processed;
}

void Component::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    if (relative_position_ == position) return;
    relative_position_ = position;
    alignment_ = alignment;
    updateGlobalPosition();
}

void Component::setVisibility(bool is_visible)
{
    is_visible_ = is_visible;
}

bool Component::isVisible()
{
    return is_visible_; 
}

const sf::Vector2f& Component::getPosition() const
{
    return relative_position_;
}

const sf::Vector2f& Component::getGlobalPosition() const
{
    return global_position_;
}

void Component::addChild(std::unique_ptr<Component> child)
{
    auto found = std::find(children_.cbegin(), children_.cend(), child);
    if (found != children_.cend())
    {
        return;
    }
    child->parent_ = this;
    child->updateGlobalPosition();

    children_.push_back(std::move(child));
}

void Component::updateGlobalPosition()
{
    sf::Vector2f offset{0.0f, 0.0f};

    switch (alignment_)
    {
        case (gui::Alignment::LEFT)     : offset.x = 0.0f; break;
        case (gui::Alignment::RIGHT)    : offset.x = - getWidth(); break;
        case (gui::Alignment::CENTERED) : offset.x = - getWidth() / 2.0f; offset.y = - getHeight() / 2.0f;  break;
    }

    if (parent_)
    {
        global_position_ = parent_->getGlobalPosition() + relative_position_ + offset;
    }
    else
    {
        global_position_ = relative_position_ + offset;
    }

    //TODO can add check if recalculation has changed position of parent to not recalculate childeren
    for (auto& child : children_)
    {
        child->updateGlobalPosition();
    }
}

}  // namespace gui