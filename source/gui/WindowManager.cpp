#include "gui/WindowManager.hpp"

#include <iterator>

#include "gui/ContextMenu.hpp"
#include "gui/Window.hpp"

namespace gui
{

WindowManager::WindowManager(const sf::Vector2f& mainWindowSize)
: activeWindowHandle_{nullptr}
, mainWindow_{}
{
    mainWindow_.setSize(mainWindowSize);
    renderTexture_.create(mainWindowSize.x, mainWindowSize.y);
}

WindowManager::~WindowManager() = default;

void WindowManager::addWindow(std::unique_ptr<Window> window)
{
    if (activeWindowHandle_)
    {
        activeWindowHandle_->disable();
    }
    activeWindowHandle_ = window.get();
    activeWindowHandle_->enable();
    mainWindow_.defocusWithAllChildren();
    window->setManager(this);
    windows_.push_front(std::move(window));
}

void WindowManager::addOverlay(std::unique_ptr<Component> overlay)
{
    if (!overlay)
    {
        return;
    }

    // If this overlay is a ContextMenu, ensure it can remove itself from the overlay list.
    // TODO: I don't like this solution so I need to think about something more generic.
    // dynamic_casts are bad, most of the time XD
    if (auto* menu = dynamic_cast<ContextMenu*>(overlay.get()))
    {
        menu->setCloseCallback([this](ContextMenu* menuPtr) {
            removeOverlay(menuPtr);
        });
    }

    // When an overlay is active we want to treat the UI as modal; prevent hover/interaction
    // with underlying windows while the overlay is visible.
    mainWindow_.forceMouseLeave();
    mainWindow_.defocusWithAllChildren();
    for (auto& window : windows_)
    {
        window->forceMouseLeave();
        window->defocusWithAllChildren();
    }

    overlays_.push_back(std::move(overlay));
}

void WindowManager::removeOverlay(Component* overlay)
{
    taskQueue_.push([this, overlay](){
        overlays_.remove_if([overlay](const std::unique_ptr<Component>& ptr) {
            return ptr.get() == overlay;
        });
    });
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    renderTexture_.clear(sf::Color{0,0,0,0});
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
    taskQueue_.executeAll();
}

MainWindow& WindowManager::mainWindow()
{
    return mainWindow_;
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
            if (activeWindowHandle_ == window) activeWindowHandle_ = nullptr;
            windowIterator = windows_.erase(windowIterator);
            continue;
        }

        if (not window->isVisible()) { windowIterator++; continue; }

        if (not window->isInside(mousePosition)) { windowIterator++; continue; }

        if (activeWindowHandle_ != window) // Replace active window
        {
            if (activeWindowHandle_) activeWindowHandle_->disable();
            activeWindowHandle_ = window;
            activeWindowHandle_->enable();

            // Make sure components in main window are defocused
            mainWindow_.defocusWithAllChildren();

            // bring window to front (rendering back to forth, so top window is at start of this list)
            windows_.splice(windows_.begin(), windows_, windowIterator);
        }

        auto result = activeWindowHandle_->receive(mouseButtonPressedEvent);

        // FIXME: I'm checking twice if window is dead (see top of this method)
        // FIXME2: Window manager knows to late that the window is closed
        //          So it is dead but stil have active handle to it
        //          For now i will work around on that but I need consider some solution.
        if (activeWindowHandle_->isDead())
        {
            windows_.remove_if([this](auto& window){ return window.get() == activeWindowHandle_;});
            activeWindowHandle_ = nullptr;
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
    if (activeWindowHandle_ and activeWindowHandle_->isActive() and not activeWindowHandle_->isDead())
    {
        result = activeWindowHandle_->receive(event);
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

    auto mousePosition = sf::Vector2f{mouseMovedEvent.position.x, mouseMovedEvent.position.y};

    gui::EventStatus status {EventStatus::NotConsumed};

    bool hasAnyWindowReceivedMouseEnterEvent{false};

    for (auto window = windows_.begin(); window != windows_.end(); ++window)
    {
        if (hasAnyWindowReceivedMouseEnterEvent)
        {
            if ((*window)->wasMouseInside())
            {
                (*window)->receive(event::MouseLeft{});
            }
            continue;
        }

        // FIXME refactor this loop body to reduce number of if statements
        bool isMouseInside = (*window)->isInside(mousePosition);
        bool wasMouseInside = (*window)->wasMouseInside();

        // Mouse still in active window
        if (wasMouseInside and isMouseInside)
        {
            hasAnyWindowReceivedMouseEnterEvent = true;
        }

        if (not wasMouseInside and isMouseInside and not hasAnyWindowReceivedMouseEnterEvent)
        {
            hasAnyWindowReceivedMouseEnterEvent = true;
            (*window)->receive(event::MouseEntered{});
        }

        if (wasMouseInside and not isMouseInside)
        {
            (*window)->receive(event::MouseLeft{});
        }

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

EventStatus WindowManager::forwardFocusChange(const event::FocusChange& focusChange)
{
    // If there is a modal overlay active, it should be the only recipient of focus traversal.
    if (not overlays_.empty())
    {
        auto* topOverlay = overlays_.back().get();
        return topOverlay->receive(focusChange);
    }

    EventStatus result{EventStatus::NotConsumed};

    // Forward event to active window
    if (activeWindowHandle_ and activeWindowHandle_->isActive() and not activeWindowHandle_->isDead())
    {
        result = activeWindowHandle_->receive(focusChange);
    }
    else
    {
        // Focus change is a bit different than other events.
        // I don't want to forward to main window if there is an active displayed one.
        result = mainWindow_.receive(focusChange);
    }
    return result;
}

EventStatus WindowManager::receive(const event::FocusChange& focusChange)
{
    auto result = forwardFocusChange(focusChange);

    if (result == EventStatus::NotConsumed)
    {
        //Try  again. It might be a last child that was already focused so this will restart from beggining.
        forwardFocusChange(focusChange);
    }

    return result;
}

}  // namespace gui
