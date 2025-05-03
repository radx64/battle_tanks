#include "gui/Layout.hpp"

#include <fmt/format.h>

namespace gui
{

void Layout::onRender(sf::RenderTexture&)
{
}

std::unique_ptr<GridLayout> GridLayout::create(size_t width, size_t height)
{
    return std::unique_ptr<GridLayout>{new GridLayout{width, height}};
}

GridLayout::GridLayout(size_t width, size_t height)
: width_{width}
, height_{height}
, columnSize_{"GridLayout::columnSize_"}
, rowSize_{"GridLayout::rowSize_"}
{
    grid_.resize(width_);
    for (auto& column : grid_)
    {
        column.resize(height_, nullptr);
    }

    columnSize_.setElementCount(width_);
    rowSize_.setElementCount(height_);
}

size_t GridLayout::getWidth() const
{
    return width_;
}

size_t GridLayout::getHeight() const
{
    return height_;
}

void GridLayout::addChild(std::unique_ptr<Component> child)
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
    logger_.error("GridLayout is full");
}


bool GridLayout::addColumn(const size_t index, const SizeConstraint& constraint)
{
    if (index > width_)
    {
        logger_.error(fmt::format("Column {} is out of bounds", index));
        return false;
    }

    width_++;
    grid_.emplace(std::begin(grid_) + index, height_);
    columnSize_.addElementAtIndex(index, constraint);
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::removeColumn(const size_t index)
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
    columnSize_.removeElementAtIndex(index);
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::addRow(const size_t index, const SizeConstraint& constraint)
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

    rowSize_.addElementAtIndex(index, constraint);
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::removeRow(const size_t index)
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
    rowSize_.removeElementAtIndex(index);
    recalculateChildrenBounds();
    return true;
}

void GridLayout::recalculateChildrenBounds()
{
    auto cellPosition = sf::Vector2f{0.f, 0.f};

    for (size_t x = 0; x < width_; x++)
    {
        for (size_t y = 0; y < height_; y++)
        {
            auto cellSize = sf::Vector2f{columnSize_.getElementSize(x), rowSize_.getElementSize(y)};
            if (grid_[x][y] != nullptr)
            {
                grid_[x][y]->setPosition(cellPosition);
                grid_[x][y]->setSize(cellSize);
            }
            cellPosition.y += cellSize.y;
        }
        cellPosition.y = 0.f;
        cellPosition.x += columnSize_.getElementSize(x);
    }
}

void GridLayout::onSizeChange()
{
    columnSize_.setTotalSize(getSize().x);
    rowSize_.setTotalSize(getSize().y);
    recalculateChildrenBounds();
}

void GridLayout::setColumnSize(const size_t index, const SizeConstraint& constraint)
{
    columnSize_.setElementSize(index, constraint); 
}
void GridLayout::setRowSize(const size_t index, const SizeConstraint& constraint)
{
    rowSize_.setElementSize(index, constraint);
}

void GridLayout::setElementAt(const size_t x, const size_t y, std::unique_ptr<Component> element)
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
Component* GridLayout::getElementAt(const size_t x, const size_t y) const
{
    if (x >= width_ || y >= height_)
    {
        logger_.error(fmt::format("Element at ({}, {}) is out of bounds", x, y));
        return nullptr;
    }
    return grid_[x][y];
}

void GridLayout::removeElementAt(const size_t x, const size_t y)
{
    setElementAt(x, y, nullptr);
}

std::unique_ptr<HorizontalLayout> HorizontalLayout::create(size_t width)
{
    return std::unique_ptr<HorizontalLayout>{new HorizontalLayout{width}};
}

void HorizontalLayout::addChild(std::unique_ptr<Component> child)
{
    layoutImpl_->addColumn(layoutImpl_->getWidth(), SizeConstraint::Auto());
    layoutImpl_->addChild(std::move(child));
}

bool HorizontalLayout::addColumn(const size_t index, const SizeConstraint& constraint)
{
    return layoutImpl_->addColumn(index, constraint);
}

bool HorizontalLayout::removeColumn(const size_t index)
{
    return layoutImpl_->removeColumn(index);
}

void HorizontalLayout::setColumnSize(const size_t index, const SizeConstraint& constraint)
{
    layoutImpl_->setColumnSize(index, constraint);
}

HorizontalLayout::HorizontalLayout(size_t width)
: layoutImpl_{nullptr}
{
    auto grid = GridLayout::create(width, 1);    
    layoutImpl_ = grid.get();

    Component::addChild(std::move(grid));
}

void HorizontalLayout::onSizeChange()
{
    layoutImpl_->setSize(getSize());
}

std::unique_ptr<VerticalLayout> VerticalLayout::create(size_t height)
{
    return std::unique_ptr<VerticalLayout>{new VerticalLayout{height}};
}

void VerticalLayout::addChild(std::unique_ptr<Component> child)
{
    layoutImpl_->addRow(layoutImpl_->getHeight(), SizeConstraint::Auto());
    layoutImpl_->addChild(std::move(child));
}

bool VerticalLayout::addRow(const size_t index, const SizeConstraint& constraint)
{
    return layoutImpl_->addRow(index, constraint);
}

bool VerticalLayout::removeRow(const size_t index)
{
    return layoutImpl_->removeRow(index);
}

void VerticalLayout::setRowSize(const size_t index, const SizeConstraint& constraint)
{
    layoutImpl_->setRowSize(index, constraint);
}

VerticalLayout::VerticalLayout(size_t height)
: layoutImpl_{nullptr}
{
    auto grid = GridLayout::create(1, height);
    layoutImpl_ = grid.get();

    Component::addChild(std::move(grid));
}

void VerticalLayout::onSizeChange()
{
    layoutImpl_->setSize(getSize());
}

}  // namespace gui
