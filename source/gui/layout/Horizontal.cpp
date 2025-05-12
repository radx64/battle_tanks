#include "gui/layout/Horizontal.hpp"

#include "gui/layout/Grid.hpp"

namespace gui::layout
{

std::unique_ptr<Horizontal> Horizontal::create(size_t width)
{
    return std::unique_ptr<Horizontal>{new Horizontal{width}};
}

void Horizontal::addChild(std::unique_ptr<Component> child)
{
    layoutImpl_->addColumn(layoutImpl_->getWidth(), Constraint::Auto());
    layoutImpl_->addChild(std::move(child));
}

bool Horizontal::addColumn(const size_t index, const Constraint& constraint)
{
    return layoutImpl_->addColumn(index, constraint);
}

bool Horizontal::removeColumn(const size_t index)
{
    return layoutImpl_->removeColumn(index);
}

void Horizontal::setColumnSize(const size_t index, const Constraint& constraint)
{
    layoutImpl_->setColumnSize(index, constraint);
}

void Horizontal::setPadding(const size_t padding)
{
    layoutImpl_->setHorizontalPadding(padding);
}

Horizontal::Horizontal(size_t width)
: layoutImpl_{nullptr}
{
    auto grid = Grid::create(width, 1);    
    layoutImpl_ = grid.get();

    Component::addChild(std::move(grid));
}

void Horizontal::onSizeChange()
{
    layoutImpl_->setSize(getSize());
}
    
}  // namespace gui::layout
