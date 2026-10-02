#include "engine/gfx/Shape.hpp"

namespace engine::gfx
{

void Shape::setSize(const engine::Vec2f& size)
{
    size_ = size;
}

const engine::Vec2f& Shape::getSize() const
{
    return size_;
}

void Shape::setPosition(const engine::Vec2f& position)
{
    position_ = position;
}

void Shape::setPosition(const float x, const float y)
{
    position_ = {x, y};
}

const engine::Vec2f& Shape::getPosition() const
{
    return position_;
}

void Shape::setOrigin(const engine::Vec2f& origin)
{
    origin_ = origin;
}

void Shape::setOrigin(const float x, const float y)
{
    origin_ = {x, y};
}

const engine::Vec2f& Shape::getOrigin() const
{
    return origin_;
}

void Shape::setRotation(const float rotation)
{
    rotation_ = rotation;
}

float Shape::getRotation() const
{
    return rotation_;
}

void Shape::setFillColor(const engine::gfx::Color& color)
{
    fillColor_ = color;
}

const engine::gfx::Color& Shape::getFillColor() const
{
    return fillColor_;
}

void Shape::setOutlineColor(const engine::gfx::Color& color)
{
    outlineColor_ = color;
}

const engine::gfx::Color& Shape::getOutlineColor() const
{
    return outlineColor_;
}

void Shape::setOutlineThickness(const float thickness)
{
    outlineThickness_ = thickness;
}

float Shape::getOutlineThickness() const
{
    return outlineThickness_;
}

engine::FloatRect Shape::getGlobalBounds() const
{
    return engine::FloatRect{
        position_.x - origin_.x,
        position_.y - origin_.y,
        size_.x,
        size_.y};
}

}  // namespace engine::gfx
