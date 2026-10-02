#pragma once

#include <cstddef>

#include "engine/Rect.hpp"
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
    void setPosition(float x, float y);
    const engine::Vec2f& getPosition() const;

    void setOrigin(const engine::Vec2f& origin);
    void setOrigin(float x, float y);
    const engine::Vec2f& getOrigin() const;

    void setRotation(float rotation);
    float getRotation() const;

    void setFillColor(const engine::gfx::Color& color);
    const engine::gfx::Color& getFillColor() const;
    void setOutlineColor(const engine::gfx::Color& color);
    const engine::gfx::Color& getOutlineColor() const;

    void setOutlineThickness(const float thickness);
    float getOutlineThickness() const;

    engine::FloatRect getGlobalBounds() const;

protected:
    engine::Vec2f size_{};
    engine::Vec2f position_{};
    engine::Vec2f origin_{};
    engine::gfx::Color fillColor_{};
    engine::gfx::Color outlineColor_{};
    float outlineThickness_{0.f};
    float rotation_{0.f};
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
    explicit CircleShape(float radius = 0.f, std::size_t pointCount = 30)
        : pointCount_(pointCount)
    {
        size_ = {radius * 2.f, radius * 2.f};
    }

    void setRadius(const float radius)
    {
        size_ = {radius * 2.f, radius * 2.f};
    }

    float getRadius() const
    {
        return size_.x * 0.5f;
    }

    void setPointCount(const std::size_t pointCount)
    {
        pointCount_ = pointCount;
    }

    std::size_t getPointCount() const
    {
        return pointCount_;
    }

private:
    std::size_t pointCount_;
};


}  // namespace engine::gfx
