#include "engine/gui/GUI.hpp"

namespace engine::gui
{

Widget* hitTestRecursive(Widget* widget, const engine::Vector2f& position)
{
    if (!widget->isVisible())
        return nullptr;

    for (const auto& child : widget->getChildren())
    {
        auto hitChild = hitTestRecursive(child.get(), position);
        if (hitChild)
            return hitChild;
    }

    if (widget->isInside(position))
        return widget;

    return nullptr;
}

Widget* getDeepestLastDescendant(Widget* node)
{
    if (!node)
        return nullptr;

    while (node->hasChildren())
        node = node->getChildren().back().get();

    return node;
}

Widget* getPrevious(Widget* node)
{
    if (!node)
        return nullptr;

    if (auto sibling = node->getPreviousSibling())
        return getDeepestLastDescendant(sibling);

    if (auto parent = node->getParent())
        return parent;

    Widget* root = node;            // start from current node
    while (root->getParent())          // climb to the top
        root = root->getParent();

    return getDeepestLastDescendant(root);
}

Widget* getNext(Widget* node)
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


GUI::GUI(const engine::Vec2f& mainWindowSize)
: windowManager_{mainWindowSize}
, logger_{"GUI"}
{
    windowManager_.setWindowCloseHandler([this](Window*) {
        onActiveWindowChanged(nullptr);
    });

    windowManager_.setOverlayCloseHandler([this](Overlay* removedOverlay) {
        onOverlayRemoval(removedOverlay);
    });

    windowManager_.mainWindow().setGui(this);
}

GUI::~GUI() = default;

void GUI::render(sf::RenderWindow& render_window)
{
    windowManager_.render(render_window);
}

void GUI::update()
{
    windowManager_.update();
}

MainWindow& GUI::mainWindow()
{
    return windowManager_.mainWindow();
}
void GUI::openWindow(std::unique_ptr<Window> window)
{
    window->setGui(this);
    windowManager_.openWindow(std::move(window));
    setFocus(windowManager_.getActiveWindow());
}

void GUI::openContextMenu(std::unique_ptr<ContextMenu> menu, const engine::Vec2f& globalPosition)
{
    menu->setGui(this);
    windowManager_.openContextMenu(std::move(menu), globalPosition);
}

void GUI::captureMouse(Widget* widget)
{
    if (mouseCapture_ != nullptr)
    {
        throw std::runtime_error("Mouse double capture. Panicking!");
    }

    mouseCapture_ = widget;
}
void GUI::releaseMouse()
{
    mouseCapture_ = nullptr;
}

EventStatus GUI::receive(const engine::events::MouseButtonPressed& mouseButtonPressedEvent)
{
    if (mouseCapture_)
    {
        return mouseCapture_->receive(mouseButtonPressedEvent);
    }

    if (windowManager_.hasOverlays())
    {
        auto* pressed = hitTestOverlaysOnly(mouseButtonPressedEvent.getPosition());
        if (pressed)
        {
            pressed_ = pressed;
            setFocus(pressed_);
            auto eventStatus = pressed_->receive(mouseButtonPressedEvent);
            updateHover(mouseButtonPressedEvent.getPosition());
            return eventStatus;
        }
        else
        {
            // Clicked outside of overlays, close all overlays and don't process click on windows below
            for (auto& overlay : windowManager_.getOverlays())
            {
                logger_.debug("Removing overlay id: " + std::to_string(overlay->getId()));
                overlay->close();
            }
            return EventStatus::Consumed;
        }
    }

    Window* clickedWindow = windowManager_.getTopWindowAtPosition(mouseButtonPressedEvent.getPosition());
    Window* oldWindow = windowManager_.getActiveWindow();
    if (clickedWindow and clickedWindow != oldWindow)
    {
        logger_.debug("Clicked window id: " + std::to_string(clickedWindow->getId()));
        // Clicked on a window that is not active, make it active
        windowManager_.setActiveWindow(clickedWindow);
        onActiveWindowChanged(clickedWindow);
    }
    
    auto* pressed = hitTestWindowsOnly(mouseButtonPressedEvent.getPosition());

    if (not pressed)
    {
        return EventStatus::NotConsumed;
    }

    pressed_ = pressed;
    setFocus(pressed_);
    
    auto eventStatus = pressed_->receive(mouseButtonPressedEvent);
    updateHover(mouseButtonPressedEvent.getPosition());
    return eventStatus;
}

EventStatus GUI::receive(const engine::gui::events::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (mouseCapture_)
    {
        return mouseCapture_->receive(mouseButtonDoublePressedEvent);
    }

    auto* pressed = hitTestWindowsOnly(mouseButtonDoublePressedEvent.getPosition());

    if (not pressed)
    {
        return EventStatus::NotConsumed;
    }

    setFocus(pressed);
    
    auto eventStatus = pressed->receive(mouseButtonDoublePressedEvent);
    updateHover(mouseButtonDoublePressedEvent.getPosition());
    return eventStatus;
}

EventStatus GUI::receive(const engine::events::MouseButtonReleased& mouseButtonReleasedEvent)
{
    updateHover(mouseButtonReleasedEvent.getPosition());

    if (mouseCapture_)
    {
        return mouseCapture_->receive(mouseButtonReleasedEvent);
    }

    if (pressed_)
    {
        pressed_->receive(mouseButtonReleasedEvent);
        pressed_ = nullptr;
    }
    
    //TODO generate click and doubleclick events later from here instead checking things in widgets / buttons

    return EventStatus::NotConsumed;
}

EventStatus GUI::receive(const engine::events::MouseMoved& mouseMovedEvent)
{
    if (mouseCapture_)
    {
        return mouseCapture_->receive(mouseMovedEvent);
    }

    return updateHover(mouseMovedEvent.getPosition());
}

EventStatus GUI::receive(const engine::gui::events::FocusChange& focusChangeEvent)
{
    Widget* activeRoot{nullptr};

    if (not windowManager_.getOverlays().empty())
    {
        activeRoot = windowManager_.getOverlays().front().get();
    }

    if (not activeRoot)
    {
        activeRoot = windowManager_.getActiveWindow();
    }

    if (not activeRoot)
    {
        activeRoot = &windowManager_.mainWindow();
    }

    if (focusChangeEvent.type == engine::gui::events::FocusChange::Type::Next)
    {
        auto* next = getNextFocusableWidget(activeRoot, focused_);
        setFocus(next);
    }
    else
    {
        auto* previous = getPreviousFocusableWidget(activeRoot, focused_);
        setFocus(previous);
    }

    return EventStatus::Consumed;
}

EventStatus GUI::receive(const engine::events::KeyPressed& keyboardKeyPressedEvent)
{
    if (focused_)
    {
        return focused_->receive(keyboardKeyPressedEvent);
    }
    return EventStatus::NotConsumed;
}

EventStatus GUI::receive(const engine::events::KeyReleased& keyboardKeyReleasedEvent)
{
    if (focused_)
    {
        return focused_->receive(keyboardKeyReleasedEvent);
    }
    return EventStatus::NotConsumed;
}

EventStatus GUI::receive(const engine::events::TextEntered& textEnteredEvent)
{
    if (focused_)
    {
        return focused_->receive(textEnteredEvent);
    }
    return EventStatus::NotConsumed;
}

Widget* GUI::hitTestWindowsOnly(const engine::Vector2f& position)
{
    Widget* hit = nullptr;

    // Try active window first, then inactive windows, and finally main window
    if (auto* activeWindow = windowManager_.getActiveWindow())
    {
        hit = hitTestRecursive(activeWindow, position);
    }

    if (not hit)
    {
        for (auto* inactiveWindow : windowManager_.getInactiveWindows())
        {
            hit = hitTestRecursive(inactiveWindow, position);
            if (hit) break;
        }
    }

    if (not hit)
    {
        // TODO: main window should be handled in some better way, maybe it should be also a window with some special properties instead of just widget
        hit = hitTestRecursive(&windowManager_.mainWindow(), position);
    }

    return hit;
}

Widget* GUI::hitTestOverlaysOnly(const engine::Vector2f& position)
{
    Widget* hit = nullptr;

    // First check overlays, they are on top of everything
    auto& overlays = windowManager_.getOverlays();
    for (auto it = overlays.rbegin(); it != overlays.rend(); ++it)
    {
        auto* overlay = it->get();
        if (overlay->isVisible() && not overlay->isDead())
        {
            hit = hitTestRecursive(overlay, position);
            if (hit)
            {
                return hit;
            }
        }
    }

    return nullptr;
}

Widget* GUI::hitTest(const engine::Vector2f& position)
{
    if (windowManager_.hasOverlays())
    {
        auto* hit = hitTestOverlaysOnly(position);
        if (hit)
        {
            return hit;
        }
    }
    else
    {
        // If there are no overlays, we can skip checking them
        // and go directly to windows, which is more efficient
        return hitTestWindowsOnly(position);
    }
    return nullptr;
}

Widget* GUI::getNextFocusableWidget(Widget* root, Widget* current)
{
    Widget* next = getNext(current);

    while (next)
    {
        if (next->isFocusable())
            return next;

        next = getNext(next);
    }

    return root;
}

Widget* GUI::getPreviousFocusableWidget(Widget* root, Widget* current)
{
    Widget* next = getPrevious(current);

    while (next)
    {
        if (next->isFocusable()) return next;

        next = getPrevious(next);
    }

    return root;
}

void GUI::setFocus(Widget* widget)
{
    if (focused_ == widget)
    {
        return;
    }

    if (focused_) focused_->defocus();
    focused_ = widget;
    if (focused_) focused_->focus();
}

EventStatus GUI::updateHover(const engine::Vector2f& position)
{
    auto* newHovered = hitTest(position);

    if (newHovered)
    {
        newHovered->receive(engine::events::MouseMoved{position});
    }

    if (newHovered == hovered_)
    {
        return EventStatus::Consumed;
    }

    if (hovered_)
    {
        hovered_->receive(engine::gui::events::MouseLeft{});
    }

    hovered_ = newHovered;

    if (hovered_)
    {
        hovered_->receive(engine::gui::events::MouseEntered{});
    }
    return EventStatus::NotConsumed;
}

void GUI::onActiveWindowChanged(Window* newActiveWindow)
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

void GUI::onOverlayRemoval(Overlay* removedOverlay)
{
    if (focused_ and focused_->getRoot() == removedOverlay)
    {
        focused_ = nullptr;
    }

    if (hovered_ and hovered_->getRoot() == removedOverlay)
    {
        hovered_ = nullptr;
    }

    if (pressed_ and pressed_->getRoot() == removedOverlay)
    {
        pressed_ = nullptr;
    }
}

}  // namespace engine::gui
