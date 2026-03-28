#pragma once

#include <functional>

#include "gui/Widget.hpp"

namespace gui::scrollbar
{

class Base : public gui::Widget
{
public:
    void onValueChange(std::function<void(float)> onChangeCallback);

protected:
    Base();
    
    void notifyChange(const float value);

    Widget* layout_ptr_;
    std::function<void(float)> onChangeCallback_;
};

}  // namespace gui::scrollbar
