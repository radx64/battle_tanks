#pragma once

#include <functional>

#include "gui/Component.hpp"

namespace gui::scrollbar
{

class Base : public gui::Component
{
public:
    void onValueChange(std::function<void(float)> onChangeCallback);

protected:
    Base();
    
    void notifyChange(const float value);

    Component* layout_ptr_;
    std::function<void(float)> onChangeCallback_;
};

}  // namespace gui::scrollbar
