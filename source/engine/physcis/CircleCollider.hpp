#ifndef ENGINE_PHYSICS_CIRCLECOLLIDER_HPP_
#define ENGINE_PHYSICS_CIRCLECOLLIDER_HPP_

#include <SFML/System.hpp>

namespace engine::physics
{

struct CircleCollider
{
    sf::Vector2f position;
    float radius;
};

}  // namespace engine::physics


#endif  // ENGINE_PHYSICS_CIRCLECOLLIDER_HPP_