#pragma once

#include <SFML/Graphics.hpp>

namespace engine{class Scene;}
namespace sf{class RenderWindow;}

namespace engine
{

class RigidBodyDebugRenderer
{
public:
static void debug(engine::Scene& scene, sf::RenderWindow& render_window);
};

}  // namespace game
