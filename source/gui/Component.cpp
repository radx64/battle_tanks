#include "gui/Component.hpp"

#include <cassert>

#include <fmt/format.h>

#include "engine/Logger.hpp"

#include "gui/Debug.hpp"

namespace gui
{

namespace
{

sf::Vector2f toVector2f(const event::MousePosition& position)
{
    return sf::Vector2f{position.x, position.y};
}

class InstanceIdGenerator
{
public:
    static uint32_t getId()
    {
        return nextInstanceId++;
    }

protected:
    static uint32_t nextInstanceId;
};

uint32_t InstanceIdGenerator::nextInstanceId = 0;

#define EMPTY_ON_METHOD(Class, Event)\
    EventStatus Class::on(const Event& )\
    { return EventStatus::NotConsumed; }

}  // namespace

Component::Component(const std::source_location location)
: localPosition_{}
, parent_{nullptr}
, childrenEventsProcessingEnabled_{true}
, children_ {}
, focusedChild_{nullptr}
, isVisible_ {true}
, wasMouseInside_{false}
, isFocused_{false}
, isFocusable_{false}
, id_{InstanceIdGenerator::getId()}
, logger_{debug::getFunctionNameOnly(location)+ " id:" + std::to_string(id_)}
{
    logger_.debug("Creating");
}

Component::~Component()
{
    logger_.debug("Destroying");
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
EventStatus Component::processEvent(const T& event)
{
    auto eventStatus = EventStatus::NotConsumed;

    // Its important to send update events in reverse order
    // as if child is rendered on top of another
    // it's better if topmost child captures event first
    // so it will not be send to back layer ones
    if (childrenEventsProcessingEnabled_)
    {
        for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
        {
            eventStatus = (*child)->receive(event);
            if (eventStatus == EventStatus::Consumed) return eventStatus;
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

    auto status = processEvent(mouseMovedEvent);
    return status;
}
EventStatus Component::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    return processEvent(mouseButtonPressedEvent);
}

EventStatus Component::receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    return processEvent(mouseButtonDoublePressedEvent);
}

EventStatus Component::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    return processEvent(mouseButtonReleasedEvent);
}

/* These two events (entered, left) are generated insde component using MouseMoveEvent*/
/* This two should not be propagated downwards to child components*/
EventStatus Component::receive(const event::MouseEntered& mouseEnteredEvent)
{
    if (not childrenEventsProcessingEnabled_) return EventStatus::NotConsumed;
    wasMouseInside_ = true;
    return on(mouseEnteredEvent);
}
EventStatus Component::receive(const event::MouseLeft& mouseLeftEvent)
{
    // FIXME: this if statement might be wrong as it also disable event processing
    // of this component. I need to think about it later
    // same in MouseEntered event
    if (not childrenEventsProcessingEnabled_) return EventStatus::NotConsumed;
    for (auto child = children_.rbegin(); child != children_.rend(); ++child  )
    {
        if ((*child)->wasMouseInside_)
        {
            (*child)->receive(mouseLeftEvent);
        }
    }

    wasMouseInside_ = false;
    return on(mouseLeftEvent);
}

EventStatus Component::receive(const event::KeyboardKeyPressed& keyboardKeyPressed)
{
    return processEvent(keyboardKeyPressed);
}

EventStatus Component::receive(const event::KeyboardKeyReleased& keyboardKeyReleased)
{
    return processEvent(keyboardKeyReleased);
}

EventStatus Component::receive(const event::TextEntered& textEntered)
{
    return processEvent(textEntered);
}

EventStatus Component::processFocusForwardEvent(const event::FocusChange& focusChange)
{
    if (focusedChild_ == nullptr and not isFocused() and isFocusable())
    {
        logger_.debug("Selecting this as a focus");
        this->focus();
        return EventStatus::Consumed;
    }

    if (isFocused() and children_.empty())
    {
        logger_.debug("No children to forward event to");
        this->defocusWithAllChildren();
        focusedChild_ = nullptr;
        return EventStatus::NotConsumed;
    }

    auto currentSelectionIt = std::end(children_);

    if (focusedChild_ != nullptr)
    {
        currentSelectionIt = std::find_if(std::begin(children_), std::end(children_),
            [this] (const auto& child) {return child.get() == focusedChild_;});
    }
    else
    {
        logger_.debug("Selecting first child");
        currentSelectionIt = std::begin(children_);
    }

    auto result = EventStatus::NotConsumed;

    while (currentSelectionIt != std::end(children_))
    {
        logger_.debug(fmt::format("Trying: {}", fmt::ptr(currentSelectionIt->get())));
        result = (*currentSelectionIt)->receive(focusChange);
        logger_.debug(fmt::format("Result from child: {}", toString(result)));
        
        if (result == EventStatus::Consumed)
        {
            focusedChild_ = currentSelectionIt->get();
            break;
        }

        currentSelectionIt = std::next(currentSelectionIt);
    }

    if (isFocused())
    {
        logger_.debug("Defocusing this element, leaving children do the work");
        defocus();
    }

    if (result == EventStatus::NotConsumed)
    {
        focusedChild_ = nullptr;
    }

    return result;
}

EventStatus Component::processFocusBackwardEvent(const event::FocusChange& focusChange)
{
    if (isFocused())
    {
        logger_.debug("Defocusing this element, leaving parent do the work");
        defocus();
        return EventStatus::NotConsumed;
    }

    auto currentSelectionIt = std::prev(std::begin(children_));

    if (focusedChild_ == nullptr)
    {
        if (children_.empty() and not isFocused() and isFocusable())
        {
            logger_.debug("No children to forward event to. Selecting this as a focus");
            this->focus();
            return EventStatus::Consumed;
        }
        else
        {
            logger_.debug("Selecting last if possible");
            currentSelectionIt = std::prev(children_.end());
        }
    }

    if (focusedChild_ != nullptr)
    {
        currentSelectionIt = std::find_if(std::begin(children_), std::end(children_),
            [this] (const auto& child) {return child.get() == focusedChild_;});
    }

    auto result = EventStatus::NotConsumed;

    while(std::distance(std::begin(children_), currentSelectionIt) >= 0)
    {
        logger_.debug(fmt::format("Trying: {}", fmt::ptr(currentSelectionIt->get())));
        result = (*currentSelectionIt)->receive(focusChange);
        logger_.debug(fmt::format("Got from child {}", toString(result)));
        
        if (result == EventStatus::Consumed) 
        {
            focusedChild_ = currentSelectionIt->get();   
            break;
        }

        currentSelectionIt = std::prev(currentSelectionIt);
    }

    if (result == EventStatus::NotConsumed)
    {
        logger_.debug("No more children to focus");
    }

    if (isFocusable() and (result == EventStatus::NotConsumed))
    {
        logger_.debug("Trying to focus this now");
        this->focus();
        result = EventStatus::Consumed;
    }
    
    if (result == EventStatus::NotConsumed)
    {
        focusedChild_ = nullptr;
    }

    return result;
}

EventStatus Component::receive(const event::FocusChange& focusChange)
{
    if (not childrenEventsProcessingEnabled_)
    {
        return EventStatus::NotConsumed;
    }

    auto result{EventStatus::NotConsumed};
    if (focusChange.type == event::FocusChange::Type::Next)
    {
        result = processFocusForwardEvent(focusChange);
    }
    else
    {
        result = processFocusBackwardEvent(focusChange);
    }

    logger_.debug(fmt::format("Returning {}", toString(result)));

    return result;
}

EventStatus Component::receive(const event::FocusLost& focusLost)
{
    return this->on(focusLost);
}

EventStatus Component::receive(const event::FocusGained& focusGained)
{
    return this->on(focusGained);
}

EMPTY_ON_METHOD(Component, event::MouseMoved);
EMPTY_ON_METHOD(Component, event::MouseButtonPressed);
EMPTY_ON_METHOD(Component, event::MouseButtonDoublePressed);
EMPTY_ON_METHOD(Component, event::MouseButtonReleased);
EMPTY_ON_METHOD(Component, event::MouseEntered);
EMPTY_ON_METHOD(Component, event::MouseLeft);

EMPTY_ON_METHOD(Component, event::KeyboardKeyPressed);
EMPTY_ON_METHOD(Component, event::KeyboardKeyReleased);
EMPTY_ON_METHOD(Component, event::TextEntered);

EMPTY_ON_METHOD(Component, event::FocusChange);
EMPTY_ON_METHOD(Component, event::FocusLost);
EMPTY_ON_METHOD(Component, event::FocusGained);

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

void Component::onParentSizeChange(const sf::Vector2f&)
{
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
    children_.emplace_back(std::move(child));
}

void Component::removeChild(const Component* child)
{
    auto found = std::find_if(children_.begin(), children_.end(),
        [child](const auto& c) { return c.get() == child; });

    if (found != children_.end())
    {
        children_.erase(found);
    }
    else
    {
        logger_.warning(fmt::format("Trying to remove child {} that is not present", fmt::ptr(child)));
    }
}

void Component::selectFocusedChild(Component* focusedChild)
{
    if (focusedChild_ != nullptr and focusedChild_ != focusedChild)
    {
        focusedChild_->defocus();
    }

    focusedChild_ = focusedChild;
}

void Component::focus()
{
    isFocused_ = true;
    //focusedElement_ = this;

    for (auto& child : children_)
    {
        child->defocusWithAllChildren();
    }

    if (parent_)
    {
        parent_->defocusChildrenExcept(this);
    }

    auto* parent = parent_;
    auto* current = this;

    while (parent != nullptr)
    {
        parent->defocus();
        parent->selectFocusedChild(current);
        current = parent;
        parent = parent->parent_;
    }

    receive(gui::event::FocusGained{});
}

void Component::defocus()
{
    if(isFocused())
    {
        isFocused_ = false;
        focusedChild_ = nullptr;
        receive(gui::event::FocusLost{});
    }
}

bool Component::isFocused() const
{
    return isFocused_;
}

void Component::defocusWithAllChildren()
{
    defocus();

    for (auto& child : children_)
    {
        child->defocusWithAllChildren();
    }

}

void Component::defocusChildrenExcept(const Component* focusedChild)
{
    for (auto& child : children_)
    {
        if (child.get() != focusedChild)
        {
            child->defocusWithAllChildren();
        }
    }

    if (parent_)
    {
        parent_->defocusChildrenExcept(this);
    }
}

bool Component::isFocusable() const
{
    return isFocusable_;
}

void Component::enableFocus()
{
    isFocusable_ = true;
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
    childrenEventsProcessingEnabled_ = false;
}
void Component::enableChildrenEvents()
{
    childrenEventsProcessingEnabled_ = true;
}

}  // namespace gui
