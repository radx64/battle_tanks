#include "gui/WindowManager.hpp"

#include <iterator>

#include "gui/Window.hpp"

namespace gui
{

WindowManager::WindowManager(const sf::Vector2f& mainWindowSize)
: active_window_handle_ {nullptr}
{
    main_window_ = std::make_unique<MainWindow>();
    main_window_->setSize(mainWindowSize);
}

WindowManager::~WindowManager() = default;

void WindowManager::addWindow(std::unique_ptr<Window> window)
{
    if (active_window_handle_)
    {
        active_window_handle_->defocus();
    }
    active_window_handle_ = window.get();
    active_window_handle_->focus();
    windows_.push_front(std::move(window));
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    main_window_->render(renderWindow);
    
    for (auto window = windows_.rbegin(); window != windows_.rend(); ++window  )
    {
        (*window)->render(renderWindow);
    }
}

MainWindow* WindowManager::mainWindow()
{
    return main_window_.get();
}

EventStatus WindowManager::receive(const event::MouseButtonPressed& mouseButtonPressedEvent)
{    
    // If not left mouse button
    if (mouseButtonPressedEvent.button != gui::event::MouseButton::Left) 
    {
        return gui::EventStatus::NotConsumed;
    }

    auto mousePosition = sf::Vector2f{mouseButtonPressedEvent.position.x, mouseButtonPressedEvent.position.y};
    
    // Find which window should be active and forward click events only there
    auto window_it = windows_.begin();

    while(window_it != windows_.end())
    {
        auto window = (*window_it).get();

        if (window->isDead())
        {
            if (active_window_handle_ == window) active_window_handle_ = nullptr;
            window_it = windows_.erase(window_it);
            continue;
        }
        
        if (not window->isVisible()) { window_it++; continue; }

        if (not window->isInside(mousePosition)) { window_it++; continue; }
        
        if (active_window_handle_ != window) // Replace active window
        {
            if (active_window_handle_) active_window_handle_->defocus();
            active_window_handle_ = window; 
            active_window_handle_->focus();
            // bring window to front (rendering back to forth, so top window is at start of this list)
            windows_.splice(windows_.begin(), windows_, window_it);
        }

        active_window_handle_->receive(mouseButtonPressedEvent);
        
        // FIXME: I'm checking twice if window is dead (see top of this method)
        if (active_window_handle_->isDead())
        {
            windows_.remove_if([this](auto& window){ return window.get() == active_window_handle_;});
            active_window_handle_ = nullptr;
        }
        return gui::EventStatus::Consumed;
        
        window_it++;
    }

    return main_window_->receive(mouseButtonPressedEvent);
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

        // Send move events to window only if mouse is over or window is beeing dragged/resized
        if ((*window)->isInside(mousePosition) or (not (*window)->isIdle()))
        {
            auto current_window_status = (*window)->receive(mouseMovedEvent);

            if (current_window_status == gui::EventStatus::Consumed) 
            {
                status = current_window_status;
                break; // Stop processing rest of the windows if current one consumed mouse event
            }
        }
    }

    if (status == EventStatus::NotConsumed)
    {
        status = main_window_->receive(mouseMovedEvent);
    }

    return status;
}

EventStatus WindowManager::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    EventStatus result{EventStatus::NotConsumed};

    //Forward event to active window
    if (active_window_handle_ && active_window_handle_->isFocused())
    {
        result = active_window_handle_->receive(mouseButtonReleasedEvent);
    }

    if (result ==  EventStatus::NotConsumed)
    {
        return main_window_->receive(mouseButtonReleasedEvent);
    }

    //There is no active window
    return gui::EventStatus::NotConsumed;  
}

}  // namespace gui