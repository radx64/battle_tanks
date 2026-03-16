#pragma once

#include <memory>
#include <vector>

namespace gui { class RadioButton; }  // namespace gui

namespace gui
{
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
