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
    void onRender(sf::RenderTexture& renderWindow) override;
};

class BaseLineLayout : public Layout
{
public:
    BaseLineLayout() = default;

    // Just in case anyone would use addChild instead of addComponent
    void addChild(std::unique_ptr<Component> child) override;

    // TODO maybe better idea would be to use Component::addChild method
    // and override it?
    void addComponent(std::unique_ptr<Component> component);

protected:
    virtual sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const = 0;
    virtual sf::Vector2f getNthChildPosition(const sf::Vector2f& child_size, size_t child_index) = 0;
    void recalculateChildrenBounds();
    void onParentSizeChange(const sf::Vector2f& parent_size) override;
    void onParentPositionChange(const sf::Vector2f& parent_position) override;
};

class HorizontalLayout : public BaseLineLayout
{
public:
    HorizontalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& child_size, size_t child_index) override;
};

class VerticalLayout : public BaseLineLayout
{
public:
    VerticalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layout_size) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& child_size, size_t child_index) override;
};

}  // namespace gui


#endif  // GUI_LAYOUT_HPP