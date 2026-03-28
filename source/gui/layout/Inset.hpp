#pragma once

#include "gui/layout/Base.hpp"
#include "gui/layout/Constraint.hpp"

namespace gui::layout
{

/*
    Inset layout is used to create a layout with a constrained size as an offset from the parent widget.
    The size of the inset is defined by the Constraint class.
    
    It can offset child widget size from its parent either by pixels or by percentage.
    In case of pixels, the inset will be the same size as the parent widget minus the offset.
    In case of percentage, the inset will be the same size as the parent widget multiplied by the percentage.
*/

class Inset : public Base
{
public:
    static std::unique_ptr<Inset> create(const Constraint& constraint);

    void set(const Constraint& constraint);

protected:
    Inset(const Constraint& constraint);
    void onSizeChange() override;

    Constraint constraint_;
};

}  // namespace gui::layout
