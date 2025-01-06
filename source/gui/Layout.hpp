#pragma once

#include "gui/Component.hpp"

namespace gui
{

class Layout : public Component
{
public:
    void onRender(sf::RenderTexture& renderWindow) override;
};

class FillLayout : public Layout
{
public:
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
    GridLayout(size_t width, size_t height);

    void addChild(std::unique_ptr<Component> child) override;
    void addNewColumn();
    void removeLastColumn();
    void addNewRow();
    void removeLastRow();
    size_t getWidth() const;
    size_t getHeight() const;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;

    void recalculateChildrenBounds();

    size_t width_;
    size_t  height_;

    std::vector<std::vector<Component*>> grid_;
};

class BaseLineLayout : public Layout
{
public:
    void addChild(std::unique_ptr<Component> child) override;

protected:
    virtual sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const = 0;
    virtual sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const = 0;
    void recalculateChildrenBounds();
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
};

class HorizontalLayout : public BaseLineLayout
{

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

class VerticalLayout : public BaseLineLayout
{

protected:
    sf::Vector2f getChildSize(const sf::Vector2f& layoutSize) const override;
    sf::Vector2f getNthChildPosition(const sf::Vector2f& childSize, size_t childIndex) const override;
};

}  // namespace gui
