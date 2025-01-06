#pragma once

#include "gui/Component.hpp"

namespace gui
{

class Layout : public Component
{
public:
    Layout() = default;
    void onRender(sf::RenderTexture& renderWindow) override;
};

class FillLayout : public Layout
{
public:
    FillLayout() = default;
    void addChild(std::unique_ptr<Component> child) override;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;
};

class GridLayout : public Layout
{
/*  TODO:
    - Add runtime change/removal of components
    - Add runtime change of grid size
    - Add support for defining grid size in percentage
*/

public:
    GridLayout(unsigned int width, unsigned int height);

    void addChild(std::unique_ptr<Component> child) override;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;

    void recalculateChildrenBounds();

    unsigned int width_;
    unsigned int height_;

    std::vector<std::vector<Component*>> grid_;
};

class BaseLineLayout : public Layout
{
public:
    BaseLineLayout() = default;

    void addChild(std::unique_ptr<Component> child) override;

protected:
    virtual sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const = 0;
    virtual sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const = 0;
    void recalculateChildrenBounds();
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
};

class HorizontalLayout : public BaseLineLayout
{
public:
    HorizontalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

class VerticalLayout : public BaseLineLayout
{
public:
    VerticalLayout() = default;

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

}  // namespace gui
