#include "engine/gui/Widget.hpp"

#include <cassert>

#include <fmt/format.h>

#include "engine/Logger.hpp"

#include "engine/gui/Debug.hpp"

namespace engine::gui
{

namespace
{
class InstanceIdGenerator
{
public:
    static uint32_t getId()
    {
        return next_instance_id++;
    }

protected:
    static uint32_t next_instance_id;
};

uint32_t InstanceIdGenerator::next_instance_id = 0;

#define EMPTY_ON_METHOD(Class, Event)\
    EventStatus Class::on(const Event& )\
    { return EventStatus::NotConsumed; }

}  // namespace

Widget::Widget(const std::source_location location)
: localPosition_{}
, bounds_{}
, parent_{nullptr}
, gui_{nullptr}
, children_ {}
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

Widget::~Widget()
{
    logger_.debug("Destroying");
}

uint32_t Widget::getId() const
{
    return id_;
}

const std::string_view Widget::getDebugName() const
{
    return debugName_;
}

void Widget::render(engine::gfx::RenderTarget& renderTarget)
{
    if (!isVisible_) return;
    onRender(renderTarget);
    for (auto& child : children_)
    {
        child->render(renderTarget);
    }
    debug::drawBounds(renderTarget, this);
}

template <typename T>
EventStatus Widget::processEvent(const T& event)
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

EventStatus Widget::receive(const engine::events::MouseMoved& mouseMovedEvent)
{
    return processEvent(mouseMovedEvent);
}

EventStatus Widget::receive(const engine::events::MouseButtonPressed& mouseButtonPressedEvent)
{
    return processEvent(mouseButtonPressedEvent);
}

EventStatus Widget::receive(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    return processEvent(mouseButtonDoublePressedEvent);
}

EventStatus Widget::receive(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent)
{
    return processEvent(mouseButtonReleasedEvent);
}

EventStatus Widget::receive(const engine::gui::events::MouseEntered& mouseEnteredEvent)
{
    return processEvent(mouseEnteredEvent);
}
EventStatus Widget::receive(const engine::gui::events::MouseLeft& mouseLeftEvent)
{
    return processEvent(mouseLeftEvent);
}

EventStatus Widget::receive(const engine::events::KeyPressed& keyboardKeyPressed)
{
    return processEvent(keyboardKeyPressed);
}

EventStatus Widget::receive(const engine::events::KeyReleased& keyboardKeyReleased)
{
    return processEvent(keyboardKeyReleased);
}

EventStatus Widget::receive(const engine::events::TextEntered& textEntered)
{
    return processEvent(textEntered);
}

void Widget::applyPendingOperations()
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

EventStatus Widget::receive(const engine::gui::events::FocusLost& focusLost)
{
    logger_.debug("Widget received focus lost event");
    return this->on(focusLost);
}

EventStatus Widget::receive(const engine::gui::events::FocusGained& focusGained)
{
    return this->on(focusGained);
}

EMPTY_ON_METHOD(Widget,  engine::events::MouseMoved);
EMPTY_ON_METHOD(Widget, engine::events::MouseButtonPressed);
EMPTY_ON_METHOD(Widget, engine::gui::events::MouseButtonDoublePressed);
EMPTY_ON_METHOD(Widget, engine::events::MouseButtonReleased);
EMPTY_ON_METHOD(Widget, engine::gui::events::MouseEntered);
EMPTY_ON_METHOD(Widget, engine::gui::events::MouseLeft);

EMPTY_ON_METHOD(Widget, engine::events::KeyPressed);
EMPTY_ON_METHOD(Widget, engine::events::KeyReleased);
EMPTY_ON_METHOD(Widget, engine::events::TextEntered);

EMPTY_ON_METHOD(Widget, engine::gui::events::FocusLost);
EMPTY_ON_METHOD(Widget, engine::gui::events::FocusGained);

#undef EMPTY_ON_METHOD

void Widget::setPosition(const engine::Vec2f& position)
{
    localPosition_ = position;
    updateGlobalPosition();

    for (auto& child : children_)
    {
        child->updateGlobalPosition();
    }
}

engine::Vec2f Widget::getSize() const
{
    return engine::Vec2f{bounds_.width, bounds_.height};
}

void Widget::setSize(const engine::Vec2f& size)
{
    bounds_.width = size.x;
    bounds_.height = size.y;
    updateGlobalPosition();
    onSizeChange();
}

void Widget::onSizeChange()
{
}

void Widget::setVisibility(bool isVisible)
{
    isVisible_ = isVisible;
}

bool Widget::isVisible() const
{
    return isVisible_;
}

const engine::Vec2f Widget::getPosition() const
{
    return localPosition_;
}

void Widget::onPositionChange()
{
}

bool Widget::isInside(const engine::Vec2f& position) const
{
    return bounds_.contains(position);
}

const engine::Vec2f Widget::getGlobalPosition() const
{
    return engine::Vec2f{bounds_.left, bounds_.top};
}

void Widget::addChild(std::unique_ptr<Widget> child)
{
    auto found = std::find(children_.cbegin(), children_.cend(), child);
    if (found != children_.cend())
    {
        throw std::runtime_error("Trying to add child object that was already added");
        return;
    }

    child->parent_ = this;
    child->updateGlobalPosition();
    child->setGui(gui_);

    if (isProcessingEvents_)
    {
        pendingChildrenToAdd_.emplace_back(std::move(child));
        return;
    }

    children_.emplace_back(std::move(child));
}

void Widget::removeChild(const Widget* child)
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

Widget* Widget::getParent() const
{
    return parent_;
}

Widget* Widget::getRoot()
{
    Widget* root = this;
    while (root->parent_ != nullptr)
    {
        root = root->parent_;
    }
    return root;
}

const std::vector<std::unique_ptr<Widget>>& Widget::getChildren() const
{
    return children_;
}

bool Widget::isFocused() const
{
    return isFocused_;
}

void Widget::defocusWithAllChildren()
{
    defocus();

    for (auto& child : children_)
    {
        child->defocusWithAllChildren();
    }

}

void Widget::defocusChildrenExcept(const Widget* focusedChild)
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

bool Widget::isFocusable() const
{
    return isFocusable_;
}

void Widget::enableFocus()
{
    isFocusable_ = true;
}

void Widget::disableFocus()
{
    isFocusable_ = false;
}

void Widget::updateGlobalPosition()
{
    engine::Vec2f globalBoundsPosition {};

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

size_t Widget::getChildrenCount() const
{
    return children_.size();
}

void Widget::focus()
{
    isFocused_ = true;
    receive(engine::gui::events::FocusGained{});
}

void Widget::defocus()
{
    isFocused_ = false;
    receive(engine::gui::events::FocusLost{});
}

Widget* Widget::getNextChild(Widget* child)
 {
    auto it = std::find_if(std::begin(children_), std::end(children_),
        [child](const auto& c) { return c.get() == child; });

    if (it != std::end(children_) and std::next(it) != std::end(children_))
    {
        return std::next(it)->get();
    }

    return nullptr;
 }

Widget* Widget::getNextSibling()
{
    if (parent_ == nullptr)
    {
        return nullptr;
    }

    return parent_->getNextChild(this);
}

Widget* Widget::getPreviousChild(Widget* child)
{
    auto it = std::find_if(std::begin(children_), std::end(children_),
        [child](const auto& c) { return c.get() == child; });

    if (it != std::end(children_) and it != std::begin(children_))
    {
        return std::prev(it)->get();
    }

    return nullptr;
}

Widget* Widget::getPreviousSibling()
{
    if (parent_ == nullptr)
    {
        return nullptr;
    }

    return parent_->getPreviousChild(this);
}

bool Widget::hasChildren() const
{
    return !children_.empty();
}

void Widget::setGui(GUI* gui)
{
    gui_ = gui;

    for (auto& child : children_)
    {
        child->setGui(gui);
    }
}

GUI& Widget::gui() const
{
    if (gui_)
        return *gui_;
    else
        throw std::runtime_error("GUI not set in widget!");
}



}  // namespace engine::gui
