#include "gui/Component.hpp"

#include "gui/Debug.hpp"

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
: localPosition_{}
, parent_{nullptr}
, canChildrenProcessEvents_{true}
, children_ {}
, isVisible_ {true}
, wasMouseInside_{false}
, isFocused_{false}
{
}

void Component::render(sf::RenderTexture& renderTexture)
{
    if (!isVisible_) return;
    onRender(renderTexture);
    for (auto& child : children_)
    {
        child->render(renderTexture); 
    }
    debug::drawBounds(renderTexture, this);
}

template <typename T>
EventStatus Component::processEvent(const T& event, bool isConsumable)
{
    auto eventStatus = EventStatus::NotConsumed;

    // Its important to send update events in reverse order
    // as if child is rendered on top of another
    // it's better if topmost child captures event first
    // so it will not be send to back layer ones
    if (canChildrenProcessEvents_)
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
        bool wasMouseInside = child->wasMouseInside_;

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
    if (not canChildrenProcessEvents_) return EventStatus::NotConsumed;
    wasMouseInside_ = true;
    return on(mouseEnteredEvent);
}
EventStatus Component::receive(const event::MouseLeft& mouseLeftEvent)
{
    if (not canChildrenProcessEvents_) return EventStatus::NotConsumed;
    for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
    {
        if ((*child)->wasMouseInside_)
        {
            (*child)->receive(mouseLeftEvent);
        }
    }

    wasMouseInside_ = false;
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

EventStatus Component::receive(const event::TextEntered& textEntered)
{
    return processEvent(textEntered, true);
}

#define EMPTY_ON_METHOD(Class, Event)\
    EventStatus Class::on(const Event& event)\
    { UNUSED(event); return EventStatus::NotConsumed; }

EMPTY_ON_METHOD(Component, event::MouseMoved);
EMPTY_ON_METHOD(Component, event::MouseButtonPressed);
EMPTY_ON_METHOD(Component, event::MouseButtonReleased);
EMPTY_ON_METHOD(Component, event::MouseEntered);
EMPTY_ON_METHOD(Component, event::MouseLeft);

EMPTY_ON_METHOD(Component, event::KeyboardKeyPressed);
EMPTY_ON_METHOD(Component, event::KeyboardKeyReleased);
EMPTY_ON_METHOD(Component, event::TextEntered);

#undef EMPTY_ON_METHOD

void Component::setPosition(const sf::Vector2f& position)
{
    localPosition_ = position;
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

void Component::onParentSizeChange(const sf::Vector2f& parentSize)
{
    UNUSED(parentSize);
};

void Component::setVisibility(bool isVisible)
{
    isVisible_ = isVisible;
}

bool Component::isVisible()
{
    return isVisible_; 
}

const sf::Vector2f Component::getPosition() const
{
    return localPosition_;
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
    return wasMouseInside_;
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
    isFocused_ = true;

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
    isFocused_ = false;

    for (auto& child : children_)
    {
        child->defocus();
    }

    onFocusLost();
}

bool Component::isFocused() const
{
    return isFocused_;
}

void Component::defocusChildrenExcept(const Component* focusedChild)
{
    for (auto& child : children_)
    {
        if (child.get() != focusedChild)
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
    sf::Vector2f globalBoundsPosition {};

    if (parent_)
    {
        globalBoundsPosition = parent_->getGlobalPosition() + getPosition();
    }
    else
    {
        globalBoundsPosition = getPosition();
    }

    if (bounds_.left != globalBoundsPosition.x or 
        bounds_.top != globalBoundsPosition.y)
    {
        bounds_.left = globalBoundsPosition.x;
        bounds_.top = globalBoundsPosition.y;

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
    canChildrenProcessEvents_ = false;
}
void Component::enableChildrenEvents()
{
    canChildrenProcessEvents_ = true;
}

}  // namespace gui
