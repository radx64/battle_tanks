#include "gui/Layout.hpp"

#include <fmt/format.h>

namespace gui
{

void Layout::onRender(sf::RenderTexture&)
{
}

void FillLayout::addChild(std::unique_ptr<Component> child)
{
    if (getChildrenCount() > 0)
    {
        logger_.error("FillLayout can have only one child");
    } 

    Component::addChild(std::move(child));
}

void FillLayout::onParentSizeChange(const sf::Vector2f& parentSize)
{
    setSize(parentSize);
}

void FillLayout::onSizeChange()
{
    for (const auto& child : children_)
    {
        child->setSize(getSize());
        child->setPosition({0.f, 0.f});
    }
}

GridLayout::GridLayout(size_t width, size_t height)
: width_{width}
, height_{height}
{
    grid_.resize(width_);
    for (auto& column : grid_)
    {
        column.resize(height_, nullptr);
    }
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
            auto fillLayoutWrapper = std::make_unique<FillLayout>();
            fillLayoutWrapper->addChild(std::move(child));
            grid_[x][y] = fillLayoutWrapper.get();
            Component::addChild(std::move(fillLayoutWrapper));
            recalculateChildrenBounds();
            return;
        }
    }
    logger_.error("GridLayout is full");
}


bool GridLayout::addColumn(const size_t position)
{
    if (position > width_)
    {
        logger_.error(fmt::format("Column {} is out of bounds", position));
        return false;
    }

    width_++;
    grid_.emplace(std::begin(grid_) + position, height_);
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::removeColumn(const size_t position)
{
    if (position >= width_)
    {
        logger_.error(fmt::format("Column {} is out of bounds", position));
        return false;
    }

    width_--;
    for (size_t y = 0; y < height_; y++)
    {
        auto* component = grid_[position][y];
        if (component != nullptr)
        {
            removeChild(component);
        }
    }
    grid_.erase(std::begin(grid_) + position);
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::addRow(const size_t position)
{
    if (position > height_)
    {
        logger_.error(fmt::format("Row {} is out of bounds", position));
        return false;
    }

    height_++;
    for (auto& column : grid_)
    {
        column.emplace(std::begin(column) + position, nullptr);
    }
    recalculateChildrenBounds();
    return true;
}

bool GridLayout::removeRow(const size_t position)
{
    if (position >= height_)
    {
        logger_.error(fmt::format("Row {} is out of bounds", position));
        return false;
    }

    height_--;
    for (auto& column : grid_)
    {
        if (column[position] != nullptr)
        {
            removeChild(column[position]);
        }
        column.erase(std::begin(column) + position);
    }
    recalculateChildrenBounds();
    return true;
}

void GridLayout::onParentSizeChange(const sf::Vector2f& parentSize)
{
    setSize(parentSize);
    recalculateChildrenBounds();
}

void GridLayout::recalculateChildrenBounds()
{
    auto layoutSize = getSize();
    auto cellSize = sf::Vector2f{layoutSize.x / width_, layoutSize.y / height_};

    for (size_t x = 0; x < width_; x++)
    {
        for (size_t y = 0; y < height_; y++)
        {
            if (grid_[x][y] == nullptr)
            {
                continue;
            }
            auto cellPosition = sf::Vector2f{x * cellSize.x, y * cellSize.y};
            grid_[x][y]->setPosition(cellPosition);
            grid_[x][y]->setSize(cellSize);
        }
    }
}

void GridLayout::onSizeChange()
{
    recalculateChildrenBounds();
}

void BaseLineLayout::addChild(std::unique_ptr<Component> child)
{
    // Fill layout is used to make an extra abstraction layer
    // especially when you try to stack layouts on each other
    // where layout always fits size of it's parent
    // Fill layout wrapper is some kind of barrier
    auto fillLayoutWrapper = std::make_unique<FillLayout>();
    fillLayoutWrapper->addChild(std::move(child));
    Component::addChild(std::move(fillLayoutWrapper));
    recalculateChildrenBounds();
}

void BaseLineLayout::recalculateChildrenBounds()
{
    auto layoutSize = getSize();

    auto childSize = getChildSize(layoutSize);

    size_t childIndex{0};
    for (auto& child : children_)
    {
        auto childPosition = getNthChildPosition(childSize, childIndex);

        child->setPosition(childPosition);
        child->setSize(childSize);
        childIndex++;
    }
}

void BaseLineLayout::onParentSizeChange(const sf::Vector2f& parentSize)
{
    // set layout size to it's parent size
    setSize(parentSize);
    // recalculate childen to resize them
    recalculateChildrenBounds();
}

sf::Vector2f HorizontalLayout::getChildSize(const sf::Vector2f& layoutSize) const
{
    auto childWidth = layoutSize.x / (float) Component::getChildrenCount();
    auto childHeight = layoutSize.y;
    return sf::Vector2f{childWidth, childHeight};
}

sf::Vector2f HorizontalLayout::getNthChildPosition(const sf::Vector2f& childSize,size_t childIndex) const
{
    float childX = childIndex * childSize.x;
    float childY = 0.f;
    return sf::Vector2f{childX, childY};
}


sf::Vector2f VerticalLayout::getChildSize(const sf::Vector2f& layoutSize) const
{
    auto childWidth = layoutSize.x;
    auto childHeight = layoutSize.y / (float) Component::getChildrenCount();
    return sf::Vector2f{childWidth, childHeight};
}

sf::Vector2f VerticalLayout::getNthChildPosition(const sf::Vector2f& childSize,size_t childIndex) const
{
    float childX = 0.f;
    float childY = childIndex * childSize.y;
    return sf::Vector2f{childX, childY};
}

}  // namespace gui
