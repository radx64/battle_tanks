#ifndef GAME_CAMERA_HPP_
#define GAME_CAMERA_HPP_

#include <SFML/Graphics.hpp>

namespace game 
{

class Camera
{
public:
    Camera(const sf::Vector2f& position, const sf::Vector2f& size);
    void setPosition(const float x, const float y);
    void move(const float x_offset, const float y_offset);
    void zoomIn();
    void zoomIn(const float x, const float y);
    void zoomOut();
    const sf::Vector2f& getPosition();
    const sf::Vector2f& getSize();
    void update();

protected:
    void alignBoundaries();
    sf::Vector2f current_position_;
    sf::Vector2f target_position_;
    const sf::Vector2f original_size_;
    sf::Vector2f current_size_;
    sf::Vector2f target_size_;
    float zoom_level_;
};

}  // namespace game

#endif  // GAME_CAMERA_HPP_
