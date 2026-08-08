#pragma once

#include <memory>
#include <vector>

namespace engine::gui { class RadioButton; }  // namespace engine::gui

namespace engine::gui
{
class RadioButtonGroup : public std::enable_shared_from_this<RadioButtonGroup>
{
public:

    static std::shared_ptr<RadioButtonGroup> create();

    void add(engine::gui::RadioButton* radioButton);
    void remove(engine::gui::RadioButton* radioButton);
    void deactivateOthers(engine::gui::RadioButton* radioButton);
    
protected:
    RadioButtonGroup() = default;

    std::vector<engine::gui::RadioButton*> radioButtons_;
};

}  // namespace engine::gui
