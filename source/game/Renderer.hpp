#ifndef GAME_RENDERER_HPP_
#define GAME_RENDERER_HPP_

#include <SFML/Graphics.hpp>

namespace game 
{

class Renderer
{
public:
    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};

}  // namespace game

#endif  // GAME_RENDERER_HPP_