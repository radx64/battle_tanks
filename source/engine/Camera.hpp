#ifndef ENGINE_CAMERA_HPP_
#define ENGINE_CAMERA_HPP_

#include <SFML/Graphics.hpp>

namespace engine 
{

class Camera
{
public:
    Camera(const sf::Vector2f& position, const sf::Vector2f& size);
    void setPosition(const float x, const float y);
    void moveX(const float x_velocity);
    void moveY(const float y_velocity);
    void move(const float x_velocity, const float y_velocity);
    void zoomIn();
    void zoomIn(const float x, const float y);
    void zoomOut();
    const sf::Vector2f& getPosition();
    const sf::Vector2f& getSize();
    void update(float timeStep);
    float getZoom();

protected:
    void alignBoundaries();
    sf::Vector2f current_position_;
    sf::Vector2f target_position_;
    sf::Vector2f velocity_;
    const sf::Vector2f original_size_;
    sf::Vector2f current_size_;
    sf::Vector2f target_size_;
    float zoom_level_;
};

}  // namespace engine

#endif  // ENGINE_CAMERA_HPP_
