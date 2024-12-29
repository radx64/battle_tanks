#include "gui/WindowManager.hpp"

#include <iterator>

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
        activeWindowHandle_->deactivate();
    }
    activeWindowHandle_ = window.get();
    activeWindowHandle_->activate();
    mainWindow_.defocusWithAllChildren();
    windows_.push_front(std::move(window));
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    renderTexture_.clear(sf::Color{0,0,0,0});
    mainWindow_.render(renderTexture_);

    for (auto window = windows_.rbegin(); window != windows_.rend(); ++window  )
    {
        (*window)->render(renderTexture_);
    }
    renderTexture_.display();
    sf::Sprite texture(renderTexture_.getTexture());
    renderWindow.draw(texture);
}

MainWindow& WindowManager::mainWindow()
{
    return mainWindow_;
}

template<class T>
EventStatus WindowManager::processMouseButton(const T& mouseButtonPressedEvent)
{
    // If not left mouse button
    // FIXME: This can't stay for too long as probably I wan't to send other events
    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left)
    {
        return gui::EventStatus::NotConsumed;
    }

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
            if (activeWindowHandle_) activeWindowHandle_->deactivate();
            activeWindowHandle_ = window;
            activeWindowHandle_->activate();
            // bring window to front (rendering back to forth, so top window is at start of this list)
            windows_.splice(windows_.begin(), windows_, windowIterator);
        }

        auto result = activeWindowHandle_->receive(mouseButtonPressedEvent);

        // FIXME: I'm checking twice if window is dead (see top of this method)
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

EventStatus WindowManager::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{
    return processMouseButton(mouseButtonPressedEvent);
}

EventStatus WindowManager::receive(const event::MouseButtonDoublePressed& mouseButtonDoublePressedEvent)
{
    return processMouseButton(mouseButtonDoublePressedEvent);
}

EventStatus WindowManager::receive(const event::MouseMoved& mouseMovedEvent)
{
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

EventStatus WindowManager::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindowHandle_ && activeWindowHandle_->isActive())
    {
        result = activeWindowHandle_->receive(mouseButtonReleasedEvent);
    }

    if (result ==  EventStatus::NotConsumed)
    {
        return mainWindow_.receive(mouseButtonReleasedEvent);
    }

    //There is no active window
    return gui::EventStatus::NotConsumed;
}

EventStatus WindowManager::receive(const event::KeyboardKeyPressed& keyboardKeyPressedEvent)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindowHandle_ && activeWindowHandle_->isActive())
    {
        result = activeWindowHandle_->receive(keyboardKeyPressedEvent);
    }

    if (result ==  EventStatus::NotConsumed)
    {
        return mainWindow_.receive(keyboardKeyPressedEvent);
    }

    return result;
}

EventStatus WindowManager::receive(const event::KeyboardKeyReleased& keyboardKeyReleasedEvent)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindowHandle_ && activeWindowHandle_->isActive())
    {
        result = activeWindowHandle_->receive(keyboardKeyReleasedEvent);
    }

    if (result == EventStatus::NotConsumed)
    {
        return mainWindow_.receive(keyboardKeyReleasedEvent);
    }

    return result;
}

EventStatus WindowManager::receive(const event::TextEntered& textEntered)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindowHandle_ && activeWindowHandle_->isActive())
    {
        result = activeWindowHandle_->receive(textEntered);
    }

    if (result == EventStatus::NotConsumed)
    {
        return mainWindow_.receive(textEntered);
    }

    return result;
}

EventStatus WindowManager::receive(const event::FocusChange& focusChange)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (activeWindowHandle_ && activeWindowHandle_->isActive())
    {
        result = activeWindowHandle_->receive(focusChange);
    }
    else
    {
        result = mainWindow_.receive(focusChange);
    }

    return result;
}

}  // namespace gui
