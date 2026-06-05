#pragma once

#include <SFML/Graphics.hpp>

namespace game::entity 
{

class Led
{
public:
    Led(float x, float y, sf::Texture& texture);
    void draw(sf::RenderWindow& renderWindow);
    void update(float timeStep);
    void setRotation(float rotation);
    
    float x_;
    float y_;

    void setColor(const sf::Color& color);
    
protected:
    const sf::Vector2f offset_{10.0f, 15.0f};  // Offset from tank center
    sf::Color color_{sf::Color::Green};
    bool isOn_{false};
    float period_{0.5f};  // TODO use types from clock to have clearly s, ms etc...;
    float timeToChangeState_;
    float rotation_{0.0f};
    sf::Sprite sprite_;
};

}  // namespace game::entity
