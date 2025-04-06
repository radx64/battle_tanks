#pragma once

#include <memory>
#include <vector>

namespace gui { class RadioButton; }  // namespace gui

namespace gui
{

// TODO: Consider whether using a shared_ptr for the radio button group is appropriate.
// This might be one of the rare cases where shared_ptr is genuinely useful,
// as the group needs to be shared among multiple radio buttons.

class RadioButtonGroup : public std::enable_shared_from_this<RadioButtonGroup>
{
public:

    static std::shared_ptr<RadioButtonGroup> create();

    void add(gui::RadioButton* radioButton);
    void remove(gui::RadioButton* radioButton);
    void deactivateOthers(gui::RadioButton* radioButton);
    
protected:
    RadioButtonGroup() = default;

    std::vector<gui::RadioButton*> radioButtons_;
};

}  // namespace gui
