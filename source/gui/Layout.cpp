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
