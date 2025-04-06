#include "gui/RadioButtonGroup.hpp"

#include "gui/RadioButton.hpp"

#include <memory>

namespace gui
{

std::shared_ptr<RadioButtonGroup> RadioButtonGroup::create()
{
    return std::shared_ptr<RadioButtonGroup>(new RadioButtonGroup{});
}

void RadioButtonGroup::add(gui::RadioButton* radioButton)
{
    radioButtons_.push_back(radioButton);
    radioButton->connectGroupHandler(shared_from_this());
}

void RadioButtonGroup::remove(gui::RadioButton* radioButton)
{
    auto it = std::remove(radioButtons_.begin(), radioButtons_.end(), radioButton);
    if (it != radioButtons_.end())
    {
        radioButtons_.erase(it);
    }
}

void RadioButtonGroup::deactivateOthers(gui::RadioButton* radioButton)
{
    for (auto& rb : radioButtons_)
    {
        if (rb == radioButton)
        {
            continue;
        }
        rb->setState(false);
    }
}
    
}  // namespace gui
