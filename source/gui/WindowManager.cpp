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
    windows_.push_back(std::move(window));
}

void WindowManager::render(sf::RenderWindow& renderWindow)
{
    for (auto window_it = windows_.begin(); window_it != windows_.end(); window_it = std::next(window_it))
    {
        Window* window = (*window_it).get();
        if (window->isDead())
        {
            window_it = windows_.erase(window_it);
            active_window_handle_ = windows_.back().get(); 
            active_window_handle_->focus();
            if (window_it == windows_.end()) return;
            continue;
        }
        window->render(renderWindow);
    }
}

bool WindowManager::update(const sf::Vector2f& mousePosition, bool isLeftClicked)
{
    auto window_it = windows_.end();
    while (window_it != windows_.begin())
    {
        window_it = std::prev(window_it);
        Window* window = (*window_it).get();
        if (window->hasFocus()) window->update(mousePosition, isLeftClicked);
        if (not window->isVisible()) continue;
        if (isLeftClicked && window->isInside(mousePosition))
        {
            // bring window to front (rendering back to forth)
            windows_.splice(windows_.end(), windows_, window_it);
            active_window_handle_->defocus();
            active_window_handle_ = window; 
            active_window_handle_->focus();
            return true;
        }
    }

    return false;
}

}  // namespace gui