#ifndef GRAPHICS_PARTICLE_SYSTEM_HPP_
#define GRAPHICS_PARTICLE_SYSTEM_HPP_

#include <memory>
#include <vector>

#include <SFML/Graphics.hpp>

#include "graphics/Particle.hpp"

namespace graphics
{

class ParticleSystem
{
public:
    ParticleSystem();
    // Temporary 'particle system' for drawing track imprints
    // TODO: This need to be moved outside so that particle system could be generic
    void addTrackImprint(const int x, const int y, const float angle);
    void drawTracks(sf::RenderWindow& renderWindow);
    void addParticle(std::unique_ptr<Particle> particle);
    void draw(sf::RenderWindow& renderWindow);
    void clear();
    void update(float timeStep);

protected:
    sf::RenderTexture target_;
    std::vector<std::unique_ptr<Particle>> active_particles_;
    std::vector<std::unique_ptr<Particle>> particles_to_spawn_;
};

}  // namespace graphics

#endif  // GRAPHICS_PARTICLE_SYSTEM_HPP_