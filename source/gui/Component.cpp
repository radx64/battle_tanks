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
, bounds_{}
, parent_{nullptr}
, childrenEventsProcessingEnabled_{true}
, children_ {}
, focusedChild_{nullptr}
, isVisible_ {true}
, isFocused_{false}
, isFocusable_{false}
, isProcessingEvents_{false}
, id_{InstanceIdGenerator::getId()}
, debugName_{debug::getFunctionNameOnly(location)+ " id:" + std::to_string(id_)}
, logger_{debug::getFunctionNameOnly(location)+ " id:" + std::to_string(id_)}
{
    logger_.debug("Creating");
}

Component::~Component()
{
    logger_.debug("Destroying");
}

uint32_t Component::getId() const
{
    return id_;
}

const std::string_view Component::getDebugName() const
{
    return debugName_;
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
    auto eventStatus = this->on(event);

    if (eventStatus == EventStatus::Consumed)
    {
        return eventStatus;
    }

    if (auto* parent = getParent())
    {
        return parent->receive(event);
    }

    return EventStatus::NotConsumed;
}

EventStatus Component::receive(const event::MouseMoved& mouseMovedEvent)
{
    return processEvent(mouseMovedEvent);
}

EventStatus Component::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    logger_.error("Mouse button pressed event received by component, id: " + std::string(getDebugName()));
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

EventStatus Component::receive(const event::MouseEntered& mouseEnteredEvent)
{
    return processEvent(mouseEnteredEvent);
}
EventStatus Component::receive(const event::MouseLeft& mouseLeftEvent)
{
    return processEvent(mouseLeftEvent);
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

void Component::applyPendingOperations()
{
    for (auto* child : pendingChildrenToRemove_)
    {
        removeChild(child);
    }
    pendingChildrenToRemove_.clear();

    for (auto&& child : pendingChildrenToAdd_)
    {
        addChild(std::move(child));
    }
    pendingChildrenToAdd_.clear();
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
}

void Component::onSizeChange()
{
}

void Component::setVisibility(bool isVisible)
{
    isVisible_ = isVisible;
}

bool Component::isVisible() const
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

bool Component::isInside(const event::MousePosition& position) const
{
    return isInside(toVector2f(position));
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

    if (isProcessingEvents_)
    {
        pendingChildrenToAdd_.emplace_back(std::move(child));
        return;
    }

    children_.emplace_back(std::move(child));
}

void Component::removeChild(const Component* child)
{
    if (isProcessingEvents_)
    {
        pendingChildrenToRemove_.push_back(child);
        return;
    }

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

Component* Component::getParent() const
{
    return parent_;
}

const std::vector<std::unique_ptr<Component>>& Component::getChildren() const
{
    return children_;
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

void Component::new_focus()
{
    isFocused_ = true;
    receive(gui::event::FocusGained{});
}

void Component::new_defocus()
{
    isFocused_ = false;
    receive(gui::event::FocusLost{});
}

Component* Component::getNextChild(Component* child)
 {
    auto it = std::find_if(std::begin(children_), std::end(children_),
        [child](const auto& c) { return c.get() == child; });

    if (it != std::end(children_) and std::next(it) != std::end(children_))
    {
        return std::next(it)->get();
    }

    return nullptr;
 }

Component* Component::getNextSibling()
{
    if (parent_ == nullptr)
    {
        return nullptr;
    }

    return parent_->getNextChild(this);
}

Component* Component::getPreviousChild(Component* child)
{
    auto it = std::find_if(std::begin(children_), std::end(children_),
        [child](const auto& c) { return c.get() == child; });

    if (it != std::end(children_) and it != std::begin(children_))
    {
        return std::prev(it)->get();
    }

    return nullptr;
}

Component* Component::getPreviousSibling()
{
    if (parent_ == nullptr)
    {
        return nullptr;
    }

    return parent_->getPreviousChild(this);
}

bool Component::hasChildren() const
{
    return !children_.empty();
}



}  // namespace gui
