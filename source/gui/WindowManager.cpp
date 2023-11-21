#include "gui/WindowManager.hpp"

#include <iterator>

#include "gui/Window.hpp"

namespace gui
{

WindowManager::WindowManager()
: active_window_handle_ {nullptr}
{}

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
    for (auto window_it = windows_.end(); window_it != windows_.begin();)
    {
        --window_it;
        (*window_it)->render(renderWindow);
    }
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
        Window* window = (*window_it).get();

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
        
        if (active_window_handle_->isDead())
        {
            windows_.remove_if([this](auto& window){ return window.get() == active_window_handle_;});
            active_window_handle_ = nullptr;
        }
        return gui::EventStatus::Consumed;
        
        window_it++;
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus WindowManager::receive(const event::MouseMoved& mouseMovedEvent)
{
    for (auto& window : windows_)
    {
        auto result = window->receive(mouseMovedEvent);
        if (result == gui::EventStatus::Consumed) return result;
    }

    return gui::EventStatus::NotConsumed;
}

EventStatus WindowManager::receive(const event::MouseButtonReleased& mouseButtonReleasedEvent)
{
    //Forward event to active window
    if (active_window_handle_ && active_window_handle_->isFocused())
    {
        return active_window_handle_->receive(mouseButtonReleasedEvent);
    }

    //There is no active window
    return gui::EventStatus::NotConsumed;  
}

}  // namespace gui