#ifndef GAME_IRENDERABLE_HPP_
#define GAME_IRENDERABLE_HPP_

#include <SFML/Graphics.hpp>

namespace game 
{

class IRenderable
{
    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};

}  // namespace game

#endif  // GAME_IRENDERABLE_HPP_