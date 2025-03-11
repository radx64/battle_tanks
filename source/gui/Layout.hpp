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
    bool addColumn(const size_t position);
    bool removeColumn(const size_t position);
    void setColumnSize(const size_t column, const float ratio);
    bool addRow(const size_t position);
    bool removeRow(const size_t position);
    void setRowSize(const size_t position, const float ratio);
    size_t getWidth() const;
    size_t getHeight() const;
    
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


class HorizontalLayout : public GridLayout
{
public:
    static std::unique_ptr<HorizontalLayout> create(size_t width = 0);

    void addChild(std::unique_ptr<Component> child) override;
    
    // TODO: I don't like this solution, consider doing something more sophisticated later
    // like reversing it and combining grid layout from two 1D layouts?
    // Same for Vertical layout
    bool addRow(const size_t position) = delete;
    bool removeRow(const size_t position) = delete;
    void setRowSize(const size_t position, const float ratio) = delete;
protected:
    HorizontalLayout(size_t width);
};

class VerticalLayout : public GridLayout
{
public:
    static std::unique_ptr<VerticalLayout> create(size_t height = 0);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t position) = delete;
    bool removeColumn(const size_t position) = delete;
    void setColumnSize(const size_t column, const float ratio) = delete;
protected:
    VerticalLayout(size_t height);
};

}  // namespace gui
