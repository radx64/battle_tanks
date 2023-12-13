#include "gui/Layout.hpp"

namespace gui
{

void Layout::onRender(sf::RenderWindow& renderWindow)
{
    static_cast<void>(renderWindow);
}

void BaseLineLayout::addChild(std::unique_ptr<Component> child)
{
    addComponent(std::move(child));
}

void BaseLineLayout::addComponent(std::unique_ptr<Component> component)
{
    Component::addChild(std::move(component));
    recalculateChildrenBounds();
}


void BaseLineLayout::recalculateChildrenBounds()
{
    auto layout_size = Component::getSize();

    auto child_size = getChildSize(layout_size);

    size_t child_index{0};
    for (auto& child : children_)
    {
        auto child_position = getNthChildPosition(child_size, child_index);

        // ignore alignment left for now (i will remove it later)
        // as using such layouts is a better idea
        child->setPosition(child_position, Alignment::LEFT);
        child->setSize(child_size);
        child_index++;
    }
}

void BaseLineLayout::onParentSizeChange(const sf::Vector2f& parent_size)
{
    // set layout size to it's parent size
    // FIXME: Maybe this setSize should be decided by parent?
    Component::setSize(parent_size);
    // recalculate childen to resize them
    recalculateChildrenBounds();
}

void BaseLineLayout::onParentPositionChange(const sf::Vector2f& parent_position)
{
    UNUSED(parent_position);
    // FIXME: Is it really needed. Position change of a parent should move layout
    // and it's children but sizes should stay the same
    recalculateChildrenBounds();
}

sf::Vector2f HorizontalLayout::getChildSize(const sf::Vector2f& layout_size) const
{
    auto child_width = layout_size.x / Component::getChildrenCount();
    auto child_height = layout_size.y;
    return sf::Vector2f{child_width, child_height};
}

sf::Vector2f HorizontalLayout::getNthChildPosition(const sf::Vector2f& child_size,size_t child_index)
{
    float child_x = child_index * child_size.x;
    float child_y = 0.f;
    return sf::Vector2f{child_x, child_y};
}


sf::Vector2f VerticalLayout::getChildSize(const sf::Vector2f& layout_size) const
{
    auto child_width = layout_size.x;
    auto child_height = layout_size.y / Component::getChildrenCount();
    return sf::Vector2f{child_width, child_height};
}

sf::Vector2f VerticalLayout::getNthChildPosition(const sf::Vector2f& child_size,size_t child_index)
{
    float child_x = 0.f;
    float child_y = child_index * child_size.y;
    return sf::Vector2f{child_x, child_y};
}

}  // namespace gui
