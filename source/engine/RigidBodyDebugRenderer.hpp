#ifndef ENGINE_RIGIDBODYDEBUGRENDERER_HPP_
#define ENGINE_RIGIDBODYDEBUGRENDERER_HPP_

#include <SFML/Graphics.hpp>

namespace engine{class Scene;}
namespace sf{class RenderWindow;}

namespace engine
{

class RigidBodyDebugRenderer
{
public:
static void debug(engine::Scene& scene, sf::RenderWindow& renderWindow);
};

}  // namespace game

#endif  // ENGINE_RIGIDBODYDEBUGRENDERER_HPP_
