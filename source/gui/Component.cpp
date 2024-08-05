#include "gui/Component.hpp"

namespace gui
{

namespace
{

sf::Vector2f toVector2f(const event::MousePosition& position)
{
    return sf::Vector2f{position.x, position.y};
}

}

Component::Component()
: local_position_{}
, parent_{nullptr}
, can_children_process_events_{true}
, children_ {}
, is_visible_ {true}
, was_mouse_inside_{false}
, is_focused_{false}
{
}

void Component::render(sf::RenderTexture& renderTexture)
{
    if (!is_visible_) return;
    onRender(renderTexture);
    for (auto& child : children_)
    {
        child->render(renderTexture); 
    }
}

template <typename T>
EventStatus Component::processEvent(const T& event, bool isConsumable)
{
    auto eventStatus = EventStatus::NotConsumed;

    // Its important to send update events in reverse order
    // as if child is rendered on top of another
    // it's better if topmost child captures event first
    // so it will not be send to back layer ones
    if (can_children_process_events_)
    {
        for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
        {
            eventStatus = (*child)->receive(event);
            if (isConsumable && eventStatus == EventStatus::Consumed) return eventStatus;
        }
    }

    return this->on(event);
}

EventStatus Component::receive(const event::MouseMoved& mouseMovedEvent)
{
    bool hasChildComponentCapturesMouseEnterEvent{false};
    for (auto& child : children_)
    {
        // FIXME refactor this loop body to reduce number of if statements
        bool isMouseInside = child->isInside(toVector2f(mouseMovedEvent.position));
        bool wasMouseInside = child->was_mouse_inside_;

        if (hasChildComponentCapturesMouseEnterEvent and wasMouseInside)
        {
            child->receive(event::MouseLeft{});
            continue; // generate mouse left event to every child that has previously mouse inside
        }

        if (not wasMouseInside and isMouseInside)
        {
            child->receive(event::MouseEntered{});
            hasChildComponentCapturesMouseEnterEvent = true;
        }

        if (wasMouseInside and not isMouseInside)
        {
            child->receive(event::MouseLeft{});
        }
    }

    auto status = processEvent(mouseMovedEvent, true);
    return status;
}
EventStatus Component::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    return processEvent(mouseButtonPressedEvent, true);
}
EventStatus Component::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    return processEvent(mouseButtonReleasedEvent, true);
}

/* These two events (entered, left) are generated insde component using MouseMoveEvent*/
/* This two should not be propagated downwards to child components*/
EventStatus Component::receive(const event::MouseEntered& mouseEnteredEvent)
{
    if (not can_children_process_events_) return EventStatus::NotConsumed;
    was_mouse_inside_ = true;
    return on(mouseEnteredEvent);
}
EventStatus Component::receive(const event::MouseLeft& mouseLeftEvent)
{
    if (not can_children_process_events_) return EventStatus::NotConsumed;
    for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
    {
        if ((*child)->was_mouse_inside_)
        {
            (*child)->receive(mouseLeftEvent);
        }
    }

    was_mouse_inside_ = false;
    return this->on(mouseLeftEvent);
}

EventStatus Component::receive(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    return processEvent(keyboardKeyPressed, true);
}

EventStatus Component::receive(const event::KeyboardKeyReleased& keyboardKeyReleased)
{
    return processEvent(keyboardKeyReleased, true);
}

EventStatus Component::on(const event::MouseMoved& mouseMovedEvent)
{
    UNUSED(mouseMovedEvent);
    return EventStatus::NotConsumed;
}
EventStatus Component::on(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    UNUSED(mouseButtonPressedEvent);
    return EventStatus::NotConsumed;
}
EventStatus Component::on(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    UNUSED(mouseButtonReleasedEvent);
    return EventStatus::NotConsumed;
}

EventStatus Component::on(const event::MouseEntered& mouseEnteredEvent)
{
    UNUSED(mouseEnteredEvent);
    return EventStatus::NotConsumed;
}
EventStatus Component::on(const event::MouseLeft& mouseLeftEvent)
{
    UNUSED(mouseLeftEvent);
    return EventStatus::NotConsumed;
}

EventStatus Component::on(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    UNUSED(keyboardKeyPressedEvent);
    return EventStatus::NotConsumed;
}

EventStatus Component::on(const event::KeyboardKeyReleased& KeyboardKeyReleased)
{
    UNUSED(KeyboardKeyReleased);
    return EventStatus::NotConsumed;
}

void Component::setPosition(const sf::Vector2f& position)
{
    local_position_ = position;
    updateGlobalPosition();

    for (auto& child : children_)
    {
        child->updateGlobalPosition();
    }
}

sf::Vector2f Component::getSize() const
{
    return sf::Vector2f{bounds_.width, bounds_.height};
}

void Component::setSize(const sf::Vector2f& size)
{
    bounds_.width = size.x;
    bounds_.height = size.y;
    updateGlobalPosition();
    onSizeChange();

    for (auto& child : children_)
    {
        child->onParentSizeChange(size);
    }
}

void Component::onSizeChange()
{
}

void Component::onParentSizeChange(const sf::Vector2f& parent_size)
{
    UNUSED(parent_size);
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

void Component::onPositionChange()
{
}

bool Component::isInside(sf::Vector2f point) const
{
    return bounds_.contains(point);
}

bool Component::isInside(const event::MousePosition& position) const\
{
    return isInside(toVector2f(position));
}

bool Component::wasMouseInside() const
{
    return was_mouse_inside_;
}

const sf::Vector2f Component::getGlobalPosition() const
{
    return sf::Vector2f{bounds_.left, bounds_.top};
}

void Component::addChild(std::unique_ptr<Component> child)
{
    auto found = std::find(children_.cbegin(), children_.cend(), child);
    if (found != children_.cend())
    {
        throw std::runtime_error("Trying to add child object that was already added");
        return;
    }
    child->parent_ = this;
    child->updateGlobalPosition();
    child->onParentSizeChange(getSize());
    children_.push_back(std::move(child));
}

void Component::focus()
{
    is_focused_ = true;

    for (auto& child : children_)
    {
        child->defocus();
    }

    if (parent_)
    {
        parent_->defocusChildrenExcept(this);
    }

    onFocus();
}

void Component::defocus()
{
    is_focused_ = false;

    for (auto& child : children_)
    {
        child->defocus();
    }

    onFocusLost();
}

bool Component::isFocused()
{
    return is_focused_;
}

void Component::defocusChildrenExcept(const Component* focused_child)
{
    for (auto& child : children_)
    {
        if (child.get() != focused_child)
        {
            child->defocus();
        }
    }

    if (parent_)
    {
        parent_->defocusChildrenExcept(this);
    }
}

void Component::onFocus()
{

}

void Component::onFocusLost()
{

}

void Component::updateGlobalPosition()
{
    sf::Vector2f global_bounds_position {};

    if (parent_)
    {
        global_bounds_position = parent_->getGlobalPosition() + getPosition();
    }
    else
    {
        global_bounds_position = getPosition();
    }

    if (bounds_.left != global_bounds_position.x or 
        bounds_.top != global_bounds_position.y)
    {
        bounds_.left = global_bounds_position.x;
        bounds_.top = global_bounds_position.y;

        onPositionChange();

        for (auto& child : children_)
        {
            // children can peek parent position for own calculations so remember to update parent first
            child->updateGlobalPosition();
        }
    }
}

size_t Component::getChildrenCount() const
{
    return children_.size();
}

void Component::disableChildrenEvents()
{
    can_children_process_events_ = false;
}
void Component::enableChildrenEvents()
{
    can_children_process_events_ = true;
}

}  // namespace gui
