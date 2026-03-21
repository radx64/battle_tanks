#include "gui/GuiController.hpp"

namespace gui
{

Component* hitTestRecursive(Component* component, const gui::event::MousePosition position)
{
    if (!component->isVisible())
        return nullptr;

    for (const auto& child : component->getChildren())
    {
        auto hitChild = hitTestRecursive(child.get(), position);
        if (hitChild)
            return hitChild;
    }

    if (component->isInside(position))
        return component;

    return nullptr;
}

Component* getPrevious(Component* node)
{
    if (!node)
        return nullptr;

    // Go down
    if (node->hasChildren())
        return node->getChildren().back().get();

    // Go sideways or up
    while (node)
    {
        if (auto sibling = node->getPreviousSibling())
            return sibling;

        node = node->getParent();
    }

    return nullptr;
}

Component* getNext(Component* node)
{
    if (!node)
        return nullptr;

    // Go down
    if (node->hasChildren())
        return node->getChildren().front().get();

    // Go sideways or up
    while (node)
    {
        if (auto sibling = node->getNextSibling())
            return sibling;

        node = node->getParent();
    }

    return nullptr;
}


GuiController::GuiController(const sf::Vector2f& mainWindowSize)
: windowManager_{mainWindowSize}
, logger_{"GuiController"}
{
    windowManager_.setWindowCloseHandler([this](Window*) {
        onActiveWindowChanged(nullptr);
    });
}

GuiController::~GuiController() = default;

void GuiController::render(sf::RenderWindow& renderWindow)
{
    windowManager_.render(renderWindow);
}

void GuiController::update()
{
    windowManager_.update();
}

MainWindow& GuiController::mainWindow()
{
    return windowManager_.mainWindow();
}
void GuiController::openWindow(std::unique_ptr<Window> window)
{
    windowManager_.openWindow(std::move(window));
}

void GuiController::openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition)
{
    windowManager_.openContextMenu(std::move(menu), globalPosition);
}

EventStatus GuiController::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    Window* clickedWindow = windowManager_.getTopWindowAtPosition(mouseButtonPressedEvent.position);
    Window* oldWindow = windowManager_.getActiveWindow();
    if (clickedWindow and clickedWindow != oldWindow)
    {
        logger_.debug("Clicked window id: " + std::to_string(clickedWindow->getId()));
        // Clicked on a window that is not active, make it active
        windowManager_.setActiveWindow(clickedWindow);
        onActiveWindowChanged(clickedWindow);
    }
    
    auto* pressed = hitTest(mouseButtonPressedEvent.position);

    if (not pressed)
    {
        return EventStatus::NotConsumed;
    }

    pressed_ = pressed;
    setFocus(pressed_);
    
    auto eventStatus = pressed_->receive(mouseButtonPressedEvent);
    updateHover(mouseButtonPressedEvent.position);
    return eventStatus;
}

EventStatus GuiController::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (pressed_)
    {
        pressed_->receive(mouseButtonReleasedEvent);
        pressed_ = nullptr;
    }

    updateHover(mouseButtonReleasedEvent.position);

    //TODO generate click and doubleclick events later from here instead checking things in compoenent / buttons

    return EventStatus::NotConsumed;
}

EventStatus GuiController::receive(const event::MouseMoved& mouseMovedEvent)
{
    return updateHover(mouseMovedEvent.position);
}

EventStatus GuiController::receive(const event::FocusChange& focusChangeEvent)
{
    Component* activeRoot = windowManager_.getActiveWindow();
    if (not activeRoot)
    {
        activeRoot = &windowManager_.mainWindow();
    }

    if (focusChangeEvent.type == event::FocusChange::Type::Next)
    {
        auto* next = getNextFocusableComponent(activeRoot, focused_);
        setFocus(next);
    }
    else
    {
        auto* previous = getPreviousFocusableComponent(activeRoot, focused_);
        setFocus(previous);
    }

    return EventStatus::Consumed;
}

EventStatus GuiController::receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (focused_)
    {
        return focused_->receive(keyboardKeyPressedEvent);
    }
    return EventStatus::NotConsumed;
}

EventStatus GuiController::receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    if (focused_)
    {
        return focused_->receive(keyboardKeyReleasedEvent);
    }
    return EventStatus::NotConsumed;
}

EventStatus GuiController::receive(const event::TextEntered& textEnteredEvent)
{
    if (focused_)
    {
        return focused_->receive(textEnteredEvent);
    }
    return EventStatus::NotConsumed;
}

Component* GuiController::hitTest(const gui::event::MousePosition position)
{
    Component* hit = nullptr;

    if (auto* activeWindow = windowManager_.getActiveWindow())
    {
        hit = hitTestRecursive(activeWindow, position);
    }
    if (not hit)
    {
        // TODO: main window should be handled in some better way, maybe it should be also a window with some special properties instead of just component
        hit = hitTestRecursive(&windowManager_.mainWindow(), position);
    }

    return hit;
}

Component* GuiController::getNextFocusableComponent(Component* root, Component* current)
{
    Component* next = getNext(current);

    while (next)
    {
        if (next->isFocusable())
            return next;

        next = getNext(next);
    }

    return root;
}

Component* GuiController::getPreviousFocusableComponent(Component* root, Component* current)
{
    Component* next = getPrevious(current);

    while (next)
    {
        if (next->isFocusable())
            return next;

        next = getPrevious(next);
    }

    return root;
}

void GuiController::setFocus(Component* component)
{
    if (focused_ == component)
    {
        return;
    }

    if (focused_) focused_->defocus();
    focused_ = component;
    if (focused_) focused_->focus();
}

EventStatus GuiController::updateHover(const gui::event::MousePosition position)
{
    auto* newHovered = hitTest(position);

    if (newHovered)
    {
        newHovered->receive(event::MouseMoved{position});
    }

    if (newHovered == hovered_)
    {
        return EventStatus::Consumed;
    }

    if (hovered_)
    {
        hovered_->receive(event::MouseLeft{});
    }

    hovered_ = newHovered;

    if (hovered_)
    {
        hovered_->receive(event::MouseEntered{});
    }
    return EventStatus::NotConsumed;
}

void GuiController::onActiveWindowChanged(Window* newActiveWindow)
{
    if (newActiveWindow)
    {
        logger_.debug("Active window changed to id: " + std::to_string(newActiveWindow->getId()));
    }
    else
    {
        logger_.debug("Active window changed to none");
    }

    if (focused_ and focused_->getRoot() != newActiveWindow)
    {
        focused_ = nullptr;
    }

    if (hovered_ and hovered_->getRoot() != newActiveWindow)
    {
        hovered_ = nullptr;
    }

    if (pressed_ and pressed_->getRoot() != newActiveWindow)
    {
        pressed_ = nullptr;
    }

}

}  // namespace gui
