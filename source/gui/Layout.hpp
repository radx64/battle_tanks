#pragma once

#include <memory>
#include <vector>

#include "gui/Component.hpp"
#include "gui/DimensionConstraintScaler.hpp"

namespace gui
{

class Layout : public Component
{
public:
    void onRender(sf::RenderTexture& renderWindow) override;
};


class GridLayout : public Layout
{
public:
    static std::unique_ptr<GridLayout> create(size_t width, size_t height);

    
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
    GridLayout(size_t width, size_t height);
    void onSizeChange() override;

    void recalculateChildrenBounds();

    size_t width_;
    size_t height_;

    DimensionConstraintScaler columnSize_;
    DimensionConstraintScaler rowSize_;

    std::vector<std::vector<Component*>> grid_;
};


class HorizontalLayout : public Layout
{
public:
    static std::unique_ptr<HorizontalLayout> create(size_t width = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t index, const SizeConstraint& constraint);
    bool removeColumn(const size_t index);
    void setColumnSize(const size_t index, const SizeConstraint& constraint);
    
protected:    
    HorizontalLayout(size_t width);
    void onSizeChange() override;

    GridLayout* layoutImpl_;
};

class VerticalLayout : public Layout
{
public:
    static std::unique_ptr<VerticalLayout> create(size_t height = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addRow(const size_t index, const SizeConstraint& constraint);
    bool removeRow(const size_t index);
    void setRowSize(const size_t index, const SizeConstraint& constraint);

protected:
    VerticalLayout(size_t height);
    void onSizeChange() override;

    GridLayout* layoutImpl_;
};

}  // namespace gui
