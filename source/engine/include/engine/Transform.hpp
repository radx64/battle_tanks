#pragma once

#include <SFML/System/Vector2.hpp>

namespace engine
{

class Transform
{
public:
    Transform() = default;
    ~Transform() = default;

    sf::Vector2f& position() { return position_; }
    const sf::Vector2f& position() const { return position_; }

    sf::Vector2f& scale() { return scale_; }
    const sf::Vector2f& scale() const { return scale_; }

    float& rotation() { return rotation_; }
    const float& rotation() const { return rotation_; }

protected:
    sf::Vector2f position_{0.f, 0.f};
    sf::Vector2f scale_{1.f, 1.f};
    float rotation_{0.f};
};

}  // namespace engine
