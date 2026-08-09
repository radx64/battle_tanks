#include "engine/gui/RadioButtonGroup.hpp"

#include "engine/gui/RadioButton.hpp"

#include <memory>

namespace engine::gui
{

std::shared_ptr<RadioButtonGroup> RadioButtonGroup::create()
{
    return std::shared_ptr<RadioButtonGroup>(new RadioButtonGroup{});
}

void RadioButtonGroup::add(engine::gui::RadioButton* radioButton)
{
    radioButtons_.push_back(radioButton);
    radioButton->connectGroupHandler(shared_from_this());
}

void RadioButtonGroup::remove(engine::gui::RadioButton* radioButton)
{
    auto it = std::remove(radioButtons_.begin(), radioButtons_.end(), radioButton);
    if (it != radioButtons_.end())
    {
        radioButtons_.erase(it);
    }
}

void RadioButtonGroup::deactivateOthers(engine::gui::RadioButton* radioButton)
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
    
}  // namespace engine::gui
