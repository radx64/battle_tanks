#pragma once

#include "engine/Vector2.hpp"
#include "engine/gfx/Color.hpp"

namespace engine::gfx
{

class Shape
{
public:
    virtual ~Shape() = default;

    void setSize(const engine::Vec2f& size);
    const engine::Vec2f& getSize() const;

    void setPosition(const engine::Vec2f& position);
    const engine::Vec2f& getPosition() const;

    void setFillColor(const engine::gfx::Color& color);
    const engine::gfx::Color& getFillColor() const;
    void setOutlineColor(const engine::gfx::Color& color);
    const engine::gfx::Color& getOutlineColor() const;

    void setOutlineThickness(const float thickness);
    float getOutlineThickness() const;

protected:
    engine::Vec2f size_;
    engine::Vec2f position_;
    engine::gfx::Color fillColor_;
    engine::gfx::Color outlineColor_;
    float outlineThickness_ = 0.f;
};

class RectangleShape : public Shape
{
public:
    RectangleShape(const engine::Vec2f& size = {0.f, 0.f})
    {
        size_ = size;
    }
};

class CircleShape : public Shape
{
public:
    CircleShape(float radius = 0.f)
    {
        size_ = {radius * 2.f, radius * 2.f};
    }
};


}  // namespace engine::gfx
