#include "gui/Layout.hpp"

#include <cassert>

namespace gui
{

void Layout::onRender(sf::RenderTexture& renderWindow)
{
    UNUSED(renderWindow);
}

void FillLayout::addChild(std::unique_ptr<Component> child)
{
    assert(getChildrenCount() == 0 && "FillLayout can have only one child object");
    Component::addChild(std::move(child));
}

void FillLayout::onParentSizeChange(const sf::Vector2f& parent_size)
{
    setSize(parent_size);
}

void FillLayout::onSizeChange()
{
    for (const auto& child : children_)
    {
        child->setSize(getSize());
        child->setPosition({0.f, 0.f});
    }
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
    auto layout_size = getSize();

    auto child_size = getChildSize(layout_size);

    size_t child_index{0};
    for (auto& child : children_)
    {
        auto child_position = getNthChildPosition(child_size, child_index);

        child->setPosition(child_position);
        child->setSize(child_size);
        child_index++;
    }
}

void BaseLineLayout::onParentSizeChange(const sf::Vector2f& parent_size)
{
    // set layout size to it's parent size
    setSize(parent_size);
    // recalculate childen to resize them
    recalculateChildrenBounds();
}

sf::Vector2f HorizontalLayout::getChildSize(const sf::Vector2f& layout_size) const
{
    auto child_width = layout_size.x / (float) Component::getChildrenCount();
    auto child_height = layout_size.y;
    return sf::Vector2f{child_width, child_height};
}

sf::Vector2f HorizontalLayout::getNthChildPosition(const sf::Vector2f& child_size,size_t child_index) const
{
    float child_x = child_index * child_size.x;
    float child_y = 0.f;
    return sf::Vector2f{child_x, child_y};
}


sf::Vector2f VerticalLayout::getChildSize(const sf::Vector2f& layout_size) const
{
    auto child_width = layout_size.x;
    auto child_height = layout_size.y / (float) Component::getChildrenCount();
    return sf::Vector2f{child_width, child_height};
}

sf::Vector2f VerticalLayout::getNthChildPosition(const sf::Vector2f& child_size,size_t child_index) const
{
    float child_x = 0.f;
    float child_y = child_index * child_size.y;
    return sf::Vector2f{child_x, child_y};
}

}  // namespace gui
