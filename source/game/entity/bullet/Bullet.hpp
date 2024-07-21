#pragma once


#include <SFML/Graphics.hpp>

#include "engine/GameObject.hpp"

namespace game::entity
{

class Bullet : public engine::GameObject
{
public:
    Bullet(float x, float y, float angle, float speed, sf::Texture& bulletBody);
    virtual ~Bullet() = default;

protected:
    void onUpdate(engine::Scene& scene, float timeStep) override;

    float lifetime_;
};

}  // namespace game::entity

