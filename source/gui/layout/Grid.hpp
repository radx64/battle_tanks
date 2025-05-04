#pragma once

#include <cstddef>

#include "gui/layout/Base.hpp"
#include "gui/DimensionConstraintScaler.hpp"

namespace gui::layout
{

class Grid : public Base
{
public:
    static std::unique_ptr<Grid> create(size_t width, size_t height);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t index, const SizeConstraint& constraint);
    bool removeColumn(const size_t index);
    void setColumnSize(const size_t column, const SizeConstraint& constraint);
    bool addRow(const size_t index, const SizeConstraint& constraint);
    bool removeRow(const size_t index);
    void setRowSize(const size_t index, const SizeConstraint& constraint);
    size_t getWidth() const;
    size_t getHeight() const;

    void setElementAt(const size_t x, const size_t y, std::unique_ptr<Component> element);
    Component* getElementAt(const size_t x, const size_t y) const;
    void removeElementAt(const size_t x, const size_t y);
    
protected:
    Grid(size_t width, size_t height);
    void onSizeChange() override;

    void recalculateChildrenBounds();

    size_t width_;
    size_t height_;

    DimensionConstraintScaler columnSize_;
    DimensionConstraintScaler rowSize_;

    std::vector<std::vector<Component*>> grid_;
};

}  // namespace gui::layout
