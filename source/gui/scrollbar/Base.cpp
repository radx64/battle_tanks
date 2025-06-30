#include "gui/scrollbar/Base.hpp"

namespace gui::scrollbar
{

void Base::onValueChange(std::function<void(float)> onChangeCallback)
{
    onChangeCallback_ = onChangeCallback;
}

Base::Base()
: layout_ptr_{nullptr}
{ }

void Base::notifyChange(const float value)
{
    if (onChangeCallback_) onChangeCallback_(value);
}

}  // namespace gui::scrollbar
