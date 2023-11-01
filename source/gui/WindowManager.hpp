#ifndef GUI_WINDOW_MANAGER_HPP_
#define GUI_WINDOW_MANAGER_HPP_

#include <list>
#include <memory>
#include <iterator>
#include <iostream>

#include <SFML/Graphics.hpp>

#include "gui/Window.hpp"

namespace gui
{

class WindowManager
{
public:
    WindowManager()
    {}

    void addWindow(std::unique_ptr<Window> window)
    {
        windows_.push_back(std::move(window));
    }

    void render(sf::RenderWindow& renderWindow)
    {
        for (auto window_it = windows_.begin(); window_it != windows_.end(); window_it = std::next(window_it))
        {
            if ((*window_it)->is_dead())
            {
                window_it = windows_.erase(window_it);
                if (window_it == windows_.end()) return;
            }
            (*window_it)->render(renderWindow);
        }
    }

    bool update(const sf::Vector2f& mousePosition, bool isLeftClicked)
    {
        auto window_iterator = windows_.end();
        while (window_iterator != windows_.begin())
        {
            window_iterator = std::prev(window_iterator);
            Window* window = (*window_iterator).get();
            window->update(mousePosition, isLeftClicked);
            if (not window->isVisible()) continue;
            if (isLeftClicked && window->isInside(mousePosition))
            {
                // bring window to front (rendering back to forth)
                windows_.splice(windows_.end(), windows_, window_iterator);
                return true;
            }
        }

        return false;
    }

protected:
    std::list<std::unique_ptr<Window>> windows_;
};

}  // namespace gui

#endif  // GUI_WINDOW_MANAGER_HPP_