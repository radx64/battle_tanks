#pragma once

#include <functional>

#include "engine/gui/Widget.hpp"

namespace engine::gui::scrollbar
{

class Base : public engine::gui::Widget
{
public:
    void onValueChange(std::function<void(float)> onChangeCallback);

protected:
    Base();
    
    void notifyChange(const float value);

    Widget* layout_ptr_;
    std::function<void(float)> onChangeCallback_;
};

}  // namespace engine::gui::scrollbar
