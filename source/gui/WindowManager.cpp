#include "gui/WindowManager.hpp"

#include <iterator>

#include "gui/ContextMenu.hpp"
#include "gui/Window.hpp"

#include "gui/Debug.hpp"

namespace gui
{

WindowManager::WindowManager(const sf::Vector2f& mainWindowSize)
: activeWindow_{nullptr}
, mainWindow_{}
, focusedComponent_{nullptr}
, logger_{"WindowManager"}
{
    mainWindow_.setSize(mainWindowSize);
    renderTexture_.create(mainWindowSize.x, mainWindowSize.y);
}

WindowManager::~WindowManager() = default;

void WindowManager::openWindow(std::unique_ptr<Window> window)
{
    window->setWindowCloseHandler([this, windowPtr = window.get()]() {
        if (activeWindow_ == windowPtr) activeWindow_ = nullptr;
        logger_.debug("Window closed, id: " + std::to_string(windowPtr->getId()));
    });

    if (activeWindow_)
    {
        activeWindow_->disable();
    }
    activeWindow_ = window.get();
    activeWindow_->enable();
    mainWindow_.defocusWithAllChildren();
    window->setManager(this);
    
    if (focusedComponent_) focusedComponent_->defocus();
    focusedComponent_ = window.get();
    focusedComponent_->focus();
    windows_.push_front(std::move(window));
}

void WindowManager::openContextMenu(std::unique_ptr<ContextMenu> menu, const sf::Vector2f& globalPosition)
{
    if (!menu)
    {
        return;
    }

    menu->setCloseCallback([this](ContextMenu* menuPtr) {
        removeOverlay(menuPtr);
    });

    menu->open(globalPosition);
    addOverlay(std::move(menu));
}

void WindowManager::addOverlay(std::unique_ptr<Component> overlay)
{
    if (!overlay)
    {
        return;
    }

    // When an overlay is active we want to treat the UI as modal; prevent hover/interaction
    // with underlying windows while the overlay is visible.
    //mainWindow_.forceMouseLeave();
    mainWindow_.defocusWithAllChildren();
    for (auto& window : windows_)
    {
        //window->forceMouseLeave();
        window->defocusWithAllChildren();
    }

    overlays_.push_back(std::move(overlay));
}

void WindowManager::removeOverlay(Component* overlay)
{
    tasksQueue_.push([this, overlay](){
        overlays_.remove_if([overlay](const std::unique_ptr<Component>& ptr) {
            return ptr.get() == overlay;
        });
    });

    // TODO: add overlay should store last mouse position
    // so when overlay is closed, mouse move event can be sent to windows under it to update their hover states
    // mainWindow_.receive(event::MouseMoved{event::MousePosition{25, 255}}); // Force mouse leave events to underlying windows
    // mainWindow_.receive(event::MouseEntered{});
    // Above is a hack as hell
    // I need to rethink how events are distributed
    // And probably do something simmilar to QT
    // Where there is a central place which handles hover, focus, clicks events to target
    // end then event bubbling propagates events to parents if not consumed
    // Hit testing for components need to be done in that central place, not in each component, so I can easily update hover states of windows under overlay when it is closed
    // Overlays could be handled as special windows, so extra handling would not be needed, and they would be part of normal event distribution system, instead of being special cased in each receive method in window manager

    // Events bubbling will have to be reversed, so they will start from target component and then propagate to parents, not the other way around as it is now, 
    // so I can stop propagation when event is consumed, and I don't have to check if event is consumed in each receive method in window manager
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    renderTexture_.clear(sf::Color{0,0,0,0});

    if (focusedComponent_)
    {
        debug::drawBox(renderTexture_, focusedComponent_->getGlobalPosition(), focusedComponent_->getSize(), sf::Color::Red, 10.f);
    }

    mainWindow_.render(renderTexture_);

    for (auto window = windows_.rbegin(); window != windows_.rend(); ++window  )
    {
        (*window)->render(renderTexture_);
    }

    for (auto& overlay : overlays_)
    {
        overlay->render(renderTexture_);
    }

    renderTexture_.display();
    textureSprite_.setTexture(renderTexture_.getTexture());
    renderWindow.draw(textureSprite_);
}

void WindowManager::update()
{
    tasksQueue_.executeAll();
}

MainWindow& WindowManager::mainWindow()
{
    return mainWindow_;
}

Window* WindowManager::getActiveWindow() const
{
    return activeWindow_;
}

void WindowManager::setActiveWindow(Window* window)
{
    if (activeWindow_ == window)
    {
        return;
    }

    if (activeWindow_)
    {
        activeWindow_->disable();
    }

    activeWindow_ = window;

    if (!window)
    {
        return;
    }

    window->enable();

    auto windowIterator = std::find_if(windows_.begin(), windows_.end(), [window](const std::unique_ptr<Window>& w) {
        return w.get() == window;
    });
    
    windows_.splice(windows_.begin(), windows_, windowIterator);
}

Window* WindowManager::getTopWindowAtPosition(const event::MousePosition position) const
{
    for (auto& window : windows_)
    {
        if (window->isInside(position))
        {
            return window.get();
        }
    }

    return nullptr;
}

template<class T>
EventStatus WindowManager::processMouseButton(const T& mouseButtonPressedEvent)
{
    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};

    // Find which window should be active and forward click events only there
    auto windowIterator = windows_.begin();

    while(windowIterator != windows_.end())
    {
        auto window = (*windowIterator).get();

        if (window->isDead())
        {
            if (activeWindow_ == window) activeWindow_ = nullptr;
            windowIterator = windows_.erase(windowIterator);
            continue;
        }

        if (not window->isVisible()) { windowIterator++; continue; }

        if (not window->isInside(mousePosition)) { windowIterator++; continue; }

        if (activeWindow_ != window) // Replace active window
        {
            if (activeWindow_) activeWindow_->disable();
            activeWindow_ = window;
            activeWindow_->enable();

            // Make sure components in main window are defocused
            mainWindow_.defocusWithAllChildren();

            // bring window to front (rendering back to forth, so top window is at start of this list)
            windows_.splice(windows_.begin(), windows_, windowIterator);
        }

        auto result = activeWindow_->receive(mouseButtonPressedEvent);

        // FIXME: I'm checking twice if window is dead (see top of this method)
        // FIXME2: Window manager knows to late that the window is closed
        //          So it is dead but stil have active handle to it
        //          For now i will work around on that but I need consider some solution.
        if (activeWindow_->isDead())
        {
            windows_.remove_if([this](auto& window){ return window.get() == activeWindow_;});
            activeWindow_ = nullptr;
        }
        return result;

        windowIterator++;
    }

    return mainWindow_.receive(mouseButtonPressedEvent);
}

template<class T>
EventStatus WindowManager::processEventWithActiveWindow(const T& event)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindow_ and activeWindow_->isActive() and not activeWindow_->isDead())
    {
        result = activeWindow_->receive(event);
    }

    if (result ==  EventStatus::NotConsumed)
    {
        return mainWindow_.receive(event);
    }

    return result;
}

EventStatus WindowManager::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    // If there are overlays present, treat the UI as modal.
    // Always deliver mouse button events to the topmost overlay so it can
    // react to clicks inside or outside its bounds (e.g. close on outside click).
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(mouseButtonPressedEvent);
        return EventStatus::Consumed;
    }

    return processMouseButton(mouseButtonPressedEvent);
}

EventStatus WindowManager::receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(mouseButtonDoublePressedEvent);
        return EventStatus::Consumed;
    }

    return processMouseButton(mouseButtonDoublePressedEvent);
}

EventStatus WindowManager::receive(const event::MouseMoved& mouseMovedEvent)
{
    // If overlays are present, treat them as modal UI and prevent underlying windows
    // from reacting to mouse movement (e.g. hover states).
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(mouseMovedEvent);
        return EventStatus::Consumed;
    }

    //auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    gui::EventStatus status {EventStatus::NotConsumed};

    bool hasAnyWindowReceivedMouseEnterEvent{false};

    for (auto window = windows_.begin(); window != windows_.end(); ++window)
    {
        if (hasAnyWindowReceivedMouseEnterEvent)
        {
            // if ((*window)->wasMouseInside())
            // {
            //     (*window)->receive(event::MouseLeft{});
            // }
            continue;
        }

        // FIXME refactor this loop body to reduce number of if statements
        //bool isMouseInside = (*window)->isInside(mousePosition);
        //bool wasMouseInside = (*window)->wasMouseInside();

        // Mouse still in active window
        // if (wasMouseInside and isMouseInside)
        // {
        //     hasAnyWindowReceivedMouseEnterEvent = true;
        // }

        // if (not wasMouseInside and isMouseInside and not hasAnyWindowReceivedMouseEnterEvent)
        // {
        //     hasAnyWindowReceivedMouseEnterEvent = true;
        //     (*window)->receive(event::MouseEntered{});
        // }

        // if (wasMouseInside and not isMouseInside)
        // {
        //     (*window)->receive(event::MouseLeft{});
        // }

        auto currentWindowStatus = (*window)->receive(mouseMovedEvent);

        if (currentWindowStatus == gui::EventStatus::Consumed)
        {
            status = currentWindowStatus;
            break; // Stop processing rest of the windows if current one consumed mouse event
        }
    }

    if (status == EventStatus::NotConsumed)
    {
        status = mainWindow_.receive(mouseMovedEvent);
    }

    return status;
}

// TODO: These receive methods are almost same, make one template (as in gui component)
EventStatus WindowManager::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(mouseButtonReleasedEvent);
        return EventStatus::Consumed;
    }

    return processEventWithActiveWindow(mouseButtonReleasedEvent);
}

EventStatus WindowManager::receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(keyboardKeyPressedEvent);
        return EventStatus::Consumed;
    }

    return processEventWithActiveWindow(keyboardKeyPressedEvent);
}

EventStatus WindowManager::receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    // FIXME: Temporary hack for new focus system,
    if (keyboardKeyReleasedEvent.key == sf::Keyboard::F4)
    {
        if (focusedComponent_ == nullptr)
        {
            // FIXME: of course windows also need to be checked :D
            focusedComponent_ = mainWindow_.getChildren().front().get();
            logger_.error("Focused component is null, setting it to " + std::to_string(focusedComponent_->getId()));
        }
        else
        {
            focusedComponent_->defocus();
            focusedComponent_ = getNextFocusableComponent(&mainWindow_, focusedComponent_);
            focusedComponent_->focus();
        }
        return EventStatus::Consumed;

    }

    if (keyboardKeyReleasedEvent.key == sf::Keyboard::F3)
    {
        if (focusedComponent_ == nullptr)
        {
            focusedComponent_ = mainWindow_.getChildren().back().get();
            logger_.error("Focused component is null, setting it to " + std::to_string(focusedComponent_->getId()));
        }
        else
        {
            focusedComponent_->defocus();
            focusedComponent_ = getPreviousFocusableComponent(&mainWindow_,focusedComponent_);
            focusedComponent_->focus();
        }
        return EventStatus::Consumed;

    }


    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(keyboardKeyReleasedEvent);
        return EventStatus::Consumed;
    }

    return processEventWithActiveWindow(keyboardKeyReleasedEvent);
}

EventStatus WindowManager::receive(const event::TextEntered& textEntered)
{
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        topOverlay->receive(textEntered);
        return EventStatus::Consumed;
    }

    return processEventWithActiveWindow(textEntered);
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

Component* WindowManager::getNextFocusableComponent(Component* root, Component* current)
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

Component* WindowManager::getPreviousFocusableComponent(Component* root, Component* current)
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

}  // namespace gui
