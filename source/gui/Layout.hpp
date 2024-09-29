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

    void onParentSizeChange(const sf::Vector2f& parentSize) override;
    void onSizeChange() override;
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
