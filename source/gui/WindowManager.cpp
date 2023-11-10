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

bool WindowManager::update(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    auto window_it = windows_.begin();

    bool has_any_window_captured_mouse_event{false};

    while(window_it != windows_.end())
    {
        Window* window = (*window_it).get();


        if (window->isFocused()) 
        {
            has_any_window_captured_mouse_event = window->update(mousePosition, isLeftClicked);
        }

        if (window->isDead())
        {
            window->setVisibility(false);
            if (active_window_handle_ == window) active_window_handle_ = nullptr;
            window_it = windows_.erase(window_it);
            continue;
        }
        else
        {
            if (not window->isVisible()) continue;
            if (isLeftClicked && window->isInside(mousePosition) && not has_any_window_captured_mouse_event )
            {
                if (active_window_handle_) active_window_handle_->defocus();
                active_window_handle_ = window; 
                active_window_handle_->focus();
                // bring window to front (rendering back to forth, so top window is at start of this list)
                windows_.splice(windows_.begin(), windows_, window_it);
                return true;
            }
            window_it++;
        }
    }

    return has_any_window_captured_mouse_event;
}

}  // namespace gui