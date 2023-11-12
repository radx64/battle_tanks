#include "gui/Component.hpp"

namespace gui
{

Component::Component()
: local_position_{}
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
    
    // Its important to send update events in reverse order
    // as if child is rendered on top of another
    // it's better if topmost child captures event first
    // so it will not be send to back layer ones
    for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
    {
        was_mouse_event_processed = (*child)->update(mousePosition, isLeftClicked);
         // child has catched the mouse event
        if (was_mouse_event_processed) return true;
    }

    // this component has catched the mouse event
    was_mouse_event_processed = onMouseUpdate(mousePosition, isLeftClicked); 

    return was_mouse_event_processed;
}

void Component::setPosition(const sf::Vector2f& position, const Alignment alignment)
{
    local_position_ = position;
    alignment_ = alignment;
    updateGlobalPosition();

    for (auto& child : children_)
    {
        child->onParentPositionChange(position);
    }
}

sf::Vector2f Component::getSize()
{
    return sf::Vector2f{bounds_.width, bounds_.height};
}

void Component::setSize(const sf::Vector2f& size)
{
    bounds_.width = size.x;
    bounds_.height = size.y;
    updateGlobalPosition();

    for (auto& child : children_)
    {
        child->onParentSizeChange(size);
    }
}

void Component::onParentSizeChange(const sf::Vector2f& parent_size)
{
    static_cast<void>(parent_size);
};

void Component::setVisibility(bool is_visible)
{
    is_visible_ = is_visible;
}

bool Component::isVisible()
{
    return is_visible_; 
}

const sf::Vector2f Component::getPosition() const
{
    return local_position_;
}

void Component::onParentPositionChange(const sf::Vector2f& parent_position)
{
    static_cast<void>(parent_position);
};

const sf::Vector2f Component::getGlobalPosition() const
{
    return sf::Vector2f{bounds_.left, bounds_.top};
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
        case (gui::Alignment::RIGHT)    : offset.x = - bounds_.width; break;
        case (gui::Alignment::CENTERED) : offset.x = - bounds_.width / 2.0f; offset.y = - bounds_.height / 2.0f;  break;
    }

    sf::Vector2f global_bounds_position {};

    if (parent_)
    {
        global_bounds_position = parent_->getGlobalPosition() + getPosition() + offset;
    }
    else
    {
        global_bounds_position = getPosition() + offset;
    }

    bounds_.left = global_bounds_position.x;
    bounds_.top = global_bounds_position.y;

    //TODO can add check if recalculation has changed position of parent to not recalculate childeren
    for (auto& child : children_)
    {
        // children can peek parent position for own calculations so remember to update parent first
        child->updateGlobalPosition();
    }
}

size_t Component::getChildrenCount() const
{
    return children_.size();
}

}  // namespace gui