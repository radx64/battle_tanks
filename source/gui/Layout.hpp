#ifndef GUI_LAYOUT_HPP_
#define GUI_LAYOUT_HPP_

#include "gui/Alignment.hpp"
#include "gui/Component.hpp"

namespace gui
{

class Layout : public Component
{
public:
    Layout() = default;
    void onRender(sf::RenderWindow& renderWindow) override
    {
        static_cast<void>(renderWindow);
    }
};

class BaseLineLayout : public Layout
{
public:
    BaseLineLayout() = default;

    // Just in case anyone would use addChild instead of addComponent
    void addChild(std::unique_ptr<Component> child) override
    {
        addComponent(std::move(child));
    }

    // TODO maybe better idea would be to use Component::addChild method
    // and override it?
    void addComponent(std::unique_ptr<Component> component)
    {
        Component::addChild(std::move(component));
        recalculateChildrenBounds();
    }

protected:
    virtual sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const = 0;
    virtual sf::Vector2f getNthChildPosition(const sf::Vector2f& child_size, size_t child_index) = 0;

    void recalculateChildrenBounds()
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

    void onParentSizeChange(const sf::Vector2f& parent_size) override
    {
        // set layout size to it's parent size
        // FIXME: Maybe this setSize should be decided by parent?
        Component::setSize(parent_size);
        // recalculate childen to resize them
        recalculateChildrenBounds();
    }

    void onParentPositionChange(const sf::Vector2f& parent_position) override
    {
        UNUSED(parent_position);
        // FIXME: Is it really needed. Position change of a parent should move layout
        // and it's children but sizes should stay the same
        recalculateChildrenBounds();
    }


};

class HorizontalLayout : public BaseLineLayout
{
public:
    HorizontalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const override
    {
        auto child_width = layout_size.x / Component::getChildrenCount();
        auto child_height = layout_size.y;
        return sf::Vector2f{child_width, child_height};
    }

    sf::Vector2f getNthChildPosition(
        const sf::Vector2f& child_size,
        size_t child_index) override
    {
        float child_x = child_index * child_size.x;
        float child_y = 0.f;
        return sf::Vector2f{child_x, child_y};
    }
};

class VerticalLayout : public BaseLineLayout
{
public:
    VerticalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const override
    {
        auto child_width = layout_size.x;
        auto child_height = layout_size.y / Component::getChildrenCount();
        return sf::Vector2f{child_width, child_height};
    }

    sf::Vector2f getNthChildPosition( 
        const sf::Vector2f& child_size,
        size_t child_index) override
    {
        float child_x = 0.f;
        float child_y = child_index * child_size.y;
        return sf::Vector2f{child_x, child_y};
    }
};

}  // namespace gui


#endif  // GUI_LAYOUT_HPP