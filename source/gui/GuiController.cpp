#include "gui/GuiController.hpp"

#include "gui/WindowManager.hpp"

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


GuiController::GuiController(WindowManager& windowManager)
: windowManager_{windowManager}
, logger_{"GuiController"}
{}

GuiController::~GuiController() = default;

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

EventStatus GuiController::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    
    Window* clickedWindow = windowManager_.getTopWindowAtPosition(mouseButtonPressedEvent.position);
    logger_.error("Clicked window id: " + std::string(clickedWindow ? clickedWindow->getDebugName() : ""));
    Window* oldWindow = windowManager_.getActiveWindow();
    if (clickedWindow and clickedWindow != oldWindow)
    {
        logger_.info("Clicked on a window that is not active, making it active");
        logger_.debug("Clicked window id: " + std::to_string(clickedWindow->getId()));
        // Clicked on a window that is not active, make it active
        windowManager_.setActiveWindow(clickedWindow);
        // TODO consider changing focuses on active window change onActiveWindowChanged event or something like that
    }
    
    auto* pressed = hitTest(mouseButtonPressedEvent.position);

    if (not pressed)
    {
        return EventStatus::NotConsumed;
    }

    pressed_ = pressed;
    pressed->focus();
    pressed_->receive(mouseButtonPressedEvent);

    return EventStatus::NotConsumed;
}

EventStatus GuiController::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (pressed_)
    {
        pressed_->receive(mouseButtonReleasedEvent);
        pressed_ = nullptr;
    }

    //TODO generate click and doubleclick events later from here instead checking things in compoenent / buttons

    return EventStatus::NotConsumed;
}

EventStatus GuiController::receive(const event::MouseMoved& mouseMovedEvent)
{
    auto* newHovered = hitTest(mouseMovedEvent.position);

    if (newHovered)
    {
        newHovered->receive(mouseMovedEvent);
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

    return EventStatus::Consumed;
}

}  // namespace gui
