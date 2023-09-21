#ifndef PARTICLES_HPP_
#define PARTICLES_HPP_

#include <SFML/Graphics.hpp>

class Particles
{
public:
    Particles();
    // Temporary 'particle system' for drawing track imprints
    void add_particle(const int x, const int y, const double angle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();

protected:
sf::RenderTexture target_;
};

#endif  // PARTICLES_HPP_