#ifndef ENGINE_PHYSICS_BOXCOLLIDER_HPP_
#define ENGINE_PHYSICS_BOXCOLLIDER_HPP_

#include <SFML/System.hpp>

namespace engine::physics
{

struct BoxCollider
{
    sf::Vector2f corners;
};

}  // namespace engine::physics


#endif  // ENGINE_PHYSICS_BOXCOLLIDER_HPP_