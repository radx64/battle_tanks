#ifndef GAME_IRENDERABLE_HPP_
#define GAME_IRENDERABLE_HPP_

#include <SFML/Graphics.hpp>

class IRenderable
{
    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};

#endif  // GAME_IRENDERABLE_HPP_