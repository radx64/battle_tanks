#include "gui/layout/Vertical.hpp"

#include "gui/layout/Grid.hpp"

namespace gui::layout
{

std::unique_ptr<Vertical> Vertical::create(size_t height)
{
    return std::unique_ptr<Vertical>{new Vertical{height}};
}

// TODO addChild and addRow have conflicting interface
// Both are adding row to the layout
void Vertical::addChild(std::unique_ptr<Component> child)
{
    layoutImpl_->addRow(layoutImpl_->getHeight(), Constraint::Auto());
    layoutImpl_->addChild(std::move(child));
}

bool Vertical::addRow(const size_t index, const Constraint& constraint)
{
    return layoutImpl_->addRow(index, constraint);
}

bool Vertical::removeRow(const size_t index)
{
    return layoutImpl_->removeRow(index);
}

void Vertical::setRowSize(const size_t index, const Constraint& constraint)
{
    layoutImpl_->setRowSize(index, constraint);
}

void Vertical::setPadding(const size_t padding)
{
    layoutImpl_->setVerticalPadding(padding);
}

Vertical::Vertical(size_t height)
: layoutImpl_{nullptr}
{
    auto grid = Grid::create(1, height);
    layoutImpl_ = grid.get();

    Component::addChild(std::move(grid));
}

void Vertical::onSizeChange()
{
    layoutImpl_->setSize(getSize());
}
    
    
}  // namespace gui::layout
