#ifndef GAME_RIGIDBODYDEBUGRENDERER_HPP_
#define GAME_RIGIDBODYDEBUGRENDERER_HPP_

#include <SFML/Graphics.hpp>

namespace game{class World;}
namespace sf{class RenderWindow;}

namespace game
{

class RigidBodyDebugRenderer
{
public:
static void debug(game::World& world, sf::RenderWindow& renderWindow);
};

}  // namespace game

#endif  // GAME_RIGIDBODYDEBUGRENDERER_HPP_
