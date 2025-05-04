#pragma once

#include <memory>

#include "gui/layout/Base.hpp"
#include "gui/DimensionConstraintScaler.hpp"  //todo move size constraint to its own file

namespace gui::layout { class Grid; }

namespace gui::layout
{

class Vertical : public Base
{
public:
    static std::unique_ptr<Vertical> create(size_t height = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addRow(const size_t index, const SizeConstraint& constraint);
    bool removeRow(const size_t index);
    void setRowSize(const size_t index, const SizeConstraint& constraint);

protected:
    Vertical(size_t height);
    void onSizeChange() override;

    Grid* layoutImpl_;
};
    
}  // namespace gui::layout
