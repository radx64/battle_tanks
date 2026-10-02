#include "engine/gfx/View.hpp"

namespace engine::gfx
{

View::View(const engine::Vec2f& center, const engine::Vec2f& size)
    : center_(center)
    , size_(size)
{
}

void View::setCenter(const engine::Vec2f& center)
{
    center_ = center;
}

void View::setSize(const engine::Vec2f& size)
{
    size_ = size;
}

const engine::Vec2f& View::getCenter() const
{
    return center_;
}

const engine::Vec2f& View::getSize() const
{
    return size_;
}

void View::zoom(const float factor)
{
    size_ *= factor;
}

void View::move(const engine::Vec2f& offset)
{
    center_ += offset;
}

}  // namespace engine::gfx
