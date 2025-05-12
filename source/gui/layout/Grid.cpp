#include "gui/layout/Grid.hpp"

#include <fmt/format.h>

namespace gui::layout
{

    std::unique_ptr<Grid> Grid::create(size_t width, size_t height)
{
    return std::unique_ptr<Grid>{new Grid{width, height}};
}

Grid::Grid(size_t width, size_t height)
: width_{width}
, height_{height}
, horizontalPadding_{0}
, verticalPadding_{0}
, columnConstraintLayout_{"Grid::columnConstraintLayout_"}
, rowConstraintLayout_{"Grid::rowConstraintLayout_"}
{
    grid_.resize(width_);
    for (auto& column : grid_)
    {
        column.resize(height_, nullptr);
    }

    columnConstraintLayout_.setElementCount(width_);
    rowConstraintLayout_.setElementCount(height_);
}

size_t Grid::getWidth() const
{
    return width_;
}

size_t Grid::getHeight() const
{
    return height_;
}

void Grid::setHorizontalPadding(const size_t padding)
{
   horizontalPadding_ = padding; 
   recalculateChildrenBounds();
}
void Grid::setVerticalPadding(const size_t padding)
{
    verticalPadding_ = padding;
    recalculateChildrenBounds();
}

void Grid::addChild(std::unique_ptr<Component> child)
{
    for (size_t y = 0; y < height_; y++)
    {
        for (size_t x = 0; x < width_; x++)
        {
            if (grid_[x][y] != nullptr)
            {
                continue;
            }
            grid_[x][y] = child.get();
            Component::addChild(std::move(child));
            recalculateChildrenBounds();
            return;
        }
    }
    logger_.error("Grid is full");
}


bool Grid::addColumn(const size_t index, const Constraint& constraint)
{
    if (index > width_)
    {
        logger_.error(fmt::format("Column {} is out of bounds", index));
        return false;
    }

    width_++;
    grid_.emplace(std::begin(grid_) + index, height_);
    columnConstraintLayout_.addElementAtIndex(index, constraint);
    recalculateChildrenBounds();
    return true;
}

bool Grid::removeColumn(const size_t index)
{
    if (index >= width_)
    {
        logger_.error(fmt::format("Column {} is out of bounds", index));
        return false;
    }

    width_--;
    for (size_t y = 0; y < height_; y++)
    {
        auto* component = grid_[index][y];
        if (component != nullptr)
        {
            removeChild(component);
        }
    }
    grid_.erase(std::begin(grid_) + index);
    columnConstraintLayout_.removeElementAtIndex(index);
    recalculateChildrenBounds();
    return true;
}

bool Grid::addRow(const size_t index, const Constraint& constraint)
{
    if (index > height_)
    {
        logger_.error(fmt::format("Row {} is out of bounds", index));
        return false;
    }

    height_++;
    for (auto& column : grid_)
    {
        column.emplace(std::begin(column) + index, nullptr);
    }

    rowConstraintLayout_.addElementAtIndex(index, constraint);
    recalculateChildrenBounds();
    return true;
}

bool Grid::removeRow(const size_t index)
{
    if (index >= height_)
    {
        logger_.error(fmt::format("Row {} is out of bounds", index));
        return false;
    }

    height_--;
    for (auto& column : grid_)
    {
        if (column[index] != nullptr)
        {
            removeChild(column[index]);
        }
        column.erase(std::begin(column) + index);
    }
    rowConstraintLayout_.removeElementAtIndex(index);
    recalculateChildrenBounds();
    return true;
}

void Grid::recalculateChildrenBounds()
{
    auto cellPosition = sf::Vector2f{(horizontalPadding_/2.f), (verticalPadding_/2.f)};

    for (size_t x = 0; x < width_; x++)
    {
        for (size_t y = 0; y < height_; y++)
        {
            auto cellSize = sf::Vector2f{columnConstraintLayout_.getElementSize(x), rowConstraintLayout_.getElementSize(y)};
            if (grid_[x][y] != nullptr)
            {
                grid_[x][y]->setPosition(cellPosition);
                grid_[x][y]->setSize(cellSize);
            }
            cellPosition.y += cellSize.y + verticalPadding_;
        }
        cellPosition.y = (verticalPadding_/2.f);
        cellPosition.x += columnConstraintLayout_.getElementSize(x)+ horizontalPadding_;
    }
}

void Grid::onSizeChange()
{
    const auto totalHorizontalPadding = horizontalPadding_ * (columnConstraintLayout_.getElementsCount());
    const auto totalVerticalPadding = verticalPadding_ * (rowConstraintLayout_.getElementsCount());

    columnConstraintLayout_.setTotalSize(getSize().x - totalHorizontalPadding);
    rowConstraintLayout_.setTotalSize(getSize().y - totalVerticalPadding);
    recalculateChildrenBounds();
}

void Grid::setColumnSize(const size_t index, const Constraint& constraint)
{
    columnConstraintLayout_.setElementSize(index, constraint); 
}
void Grid::setRowSize(const size_t index, const Constraint& constraint)
{
    rowConstraintLayout_.setElementSize(index, constraint);
}

void Grid::setElementAt(const size_t x, const size_t y, std::unique_ptr<Component> element)
{
    if (x >= width_ || y >= height_)
    {
        logger_.error(fmt::format("Element at ({}, {}) is out of bounds", x, y));
        return;
    }

    if (grid_[x][y] != nullptr)
    {
        removeChild(grid_[x][y]);
    }

    if (element == nullptr)
    {
        grid_[x][y] = nullptr;
        return;
    }
    else
    {
        grid_[x][y] = element.get();
        Component::addChild(std::move(element));
    }
    recalculateChildrenBounds();
}
Component* Grid::getElementAt(const size_t x, const size_t y) const
{
    if (x >= width_ || y >= height_)
    {
        logger_.error(fmt::format("Element at ({}, {}) is out of bounds", x, y));
        return nullptr;
    }
    return grid_[x][y];
}

void Grid::removeElementAt(const size_t x, const size_t y)
{
    setElementAt(x, y, nullptr);
}

}  // namespace gui::layout
