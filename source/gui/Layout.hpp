#pragma once

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
    GridLayout(size_t width, size_t height);

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
    HorizontalLayout();
    HorizontalLayout(size_t width);
    void addChild(std::unique_ptr<Component> child) override;

    // TODO: I don't like this solution, consider doing something more sophisticated later
    // like reversing it and combining grid layout from two 1D layouts?
    // Same for Vertical layout
    bool addRow(const size_t position) = delete;
    bool removeRow(const size_t position) = delete;
    void setRowSize(const size_t position, const float ratio) = delete;
};

class VerticalLayout : public GridLayout
{
public:
    VerticalLayout();
    VerticalLayout(size_t height);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t position) = delete;
    bool removeColumn(const size_t position) = delete;
    void setColumnSize(const size_t column, const float ratio) = delete;
};

}  // namespace gui
