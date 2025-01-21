#pragma once

#include "gui/Component.hpp"
#include "gui/DimensionConstraintScaler.hpp"

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
public:
    GridLayout(size_t width, size_t height);

    void addChild(std::unique_ptr<Component> child) override;
    bool addColumn(const size_t position);
    bool removeColumn(const size_t position);
    void setColumnSize(const size_t column, const float ratio);
    bool addRow(const size_t position);
    bool removeRow(const size_t position);
    void setRowSize(const size_t position, const float ratio);
    size_t getWidth() const;
    size_t getHeight() const;

protected:
    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;

    void recalculateChildrenBounds();

    size_t width_;
    size_t height_;

    DimensionConstraintScaler columnSize_;
    DimensionConstraintScaler rowSize_;

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
