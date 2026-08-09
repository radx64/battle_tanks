#include "engine/Transform.hpp"

namespace engine
{

engine::Vector2f& Transform::position()
{
    return position_;
}

const engine::Vector2f& Transform::position() const 
{
    return position_;
}

engine::Vector2f& Transform::scale()
{
    return scale_;
}
const engine::Vector2f& Transform::scale() const 
{
    return scale_; 
}

float& Transform::rotation()
{
    return rotation_;
}

const float& Transform::rotation() const 
{
    return rotation_;
}

}  // namespace engine
