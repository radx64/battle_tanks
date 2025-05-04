#pragma once

#include <memory>

#include "gui/layout/Base.hpp"
#include "gui/layout/SizeConstraint.hpp"

namespace gui::layout { class Grid; }

namespace gui::layout
{

class Horizontal : public Base
{
public:
    static std::unique_ptr<Horizontal> create(size_t width = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t index, const SizeConstraint& constraint);
    bool removeColumn(const size_t index);
    void setColumnSize(const size_t index, const SizeConstraint& constraint);
    
protected:    
    Horizontal(size_t width);
    void onSizeChange() override;

    Grid* layoutImpl_;
};

}  // namespace gui::layout
