#pragma once

#include "gui/layout/Base.hpp"
#include "gui/layout/SizeConstraint.hpp"

namespace gui::layout
{

/*
    Inset layout is used to create a layout with a fixed size
    and an offset from the parent component.
    It is used to create a layout with a fixed size and an offset
    from the parent component.
    The size of the inset is defined by the SizeConstraint class.
    
    It can offset child component size from its parent either by pixels or by percentage.
    In case of pixels, the inset will be the same size as the parent component minus the offset.
    In case of percentage, the inset will be the same size as the parent component multiplied by the percentage.
*/

class Inset : public Base
{
public:
    static std::unique_ptr<Inset> create(const SizeConstraint& constraint);

    void set(const SizeConstraint& constraint);

protected:
    Inset(const SizeConstraint& constraint);
    void onSizeChange() override;

    SizeConstraint constraint_;
};

}  // namespace gui::layout
