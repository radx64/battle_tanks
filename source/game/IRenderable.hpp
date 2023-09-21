#ifndef IRENDERABLE_HPP_
#define IRENDERABLE_HPP_

#include <SFML/Graphics.hpp>

class IRenderable
{
    virtual void draw(sf::RenderWindow& renderWindow) = 0;
};

#endif