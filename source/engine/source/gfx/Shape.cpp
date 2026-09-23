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

const engine::Vec2f& Shape::getPosition() const
{
    return position_;
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

}  // namespace engine::gfx
