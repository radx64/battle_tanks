#pragma once

#include <memory>

#include "gui/layout/Base.hpp"
#include "gui/layout/Constraint.hpp"

namespace gui::layout { class Grid; }

namespace gui::layout
{

// TODO: Vertical and Horizontal layouts are now braking child / parent relationship
// as there is a Grid in between. Think later if this is a good idea or not.
// It reuses grid logic but might render unexpected reults if one is traversing
// parent / child tree.

class Vertical : public Base
{
public:
    static std::unique_ptr<Vertical> create(size_t height = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addRow(const size_t index, const Constraint& constraint);
    bool removeRow(const size_t index);
    void setRowSize(const size_t index, const Constraint& constraint);
    void setPadding(const size_t padding);

protected:
    Vertical(size_t height);
    void onSizeChange() override;

    Grid* layoutImpl_;
};
    
}  // namespace gui::layout
