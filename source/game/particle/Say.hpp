#pragma once

#include <string_view>
#include <SFML/Graphics.hpp>

#include "engine/Particle.hpp"

namespace game::particle
{

class Say : public engine::Particle
{
public:
    Say(const std::string_view text, float x, float y);
    virtual ~Say() = default;

    void draw(sf::RenderWindow& renderWindow) override;

protected:
    void onUpdate(float timeStep) override;

private:
    float lifetime_;
    sf::Text text_;
};

}  // namespace game::particle
