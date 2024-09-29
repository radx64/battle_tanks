#pragma once

#include <SFML/Graphics.hpp>

namespace engine 
{

class Camera
{
public:
    Camera(const sf::Vector2f& position, const sf::Vector2f& size);
    void setPosition(const float x, const float y);
    void moveX(const float xVelocity);
    void moveY(const float yVelocity);
    void move(const float xVelocity, const float yVelocity);
    void zoomIn();
    void zoomIn(const float x, const float y);
    void zoomOut();
    const sf::Vector2f& getPosition();
    const sf::Vector2f& getSize();
    void update(float timeStep);
    float getZoom();

protected:
    void alignBoundaries();
    sf::Vector2f currentPosition_;
    sf::Vector2f targetPosition_;
    sf::Vector2f velocity_;
    const sf::Vector2f originalSize_;
    sf::Vector2f currentSize_;
    sf::Vector2f targetSize_;
    float zoomLevel_;
};

}  // namespace engine
