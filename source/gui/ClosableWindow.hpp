#ifndef GUI_CLOSABLE_WINDOW_HPP_
#define GUI_CLOSABLE_WINDOW_HPP_

#include "gui/Window.hpp"

namespace gui
{

class ClosableWindow : public Window
{
public:
    ClosableWindow(Component* parent);
};

}  // namespace gui

#endif  // GUI_CLOSABLE_WINDOW_HPP_