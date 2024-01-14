#include "game/Camera.hpp"

namespace game 
{

constexpr float MOVE_FACTOR = 0.1;
constexpr float ZOOM_FACTOR = 0.1;

constexpr float MIN_ZOOM_LEVEL = 1;
constexpr float MAX_ZOOM_LEVEL = 3;

Camera::Camera(const sf::Vector2f& position, const sf::Vector2f& size)
: current_position_{position}
, target_position_{position}
, original_size_{size}
, current_size_{size}
, target_size_{size}
, zoom_level_{MIN_ZOOM_LEVEL}
{}

void Camera::setPosition(const float x, const float y)
{
    target_position_.x = x;
    target_position_.y = y;
    alignBoundaries();
}

void Camera::alignBoundaries()
{
    auto half_of_target_size = target_size_/2.0f;
    if (target_position_.x - half_of_target_size.x < 0) target_position_.x = half_of_target_size.x;
    if (target_position_.y - half_of_target_size.y < 0) target_position_.y = half_of_target_size.y;

    if (target_position_.x + half_of_target_size.x > original_size_.x) target_position_.x = original_size_.x - half_of_target_size.x;
    if (target_position_.y + half_of_target_size.y > original_size_.y) target_position_.y = original_size_.y - half_of_target_size.y;
}

void Camera::move(const float x_offset, const float y_offset)
{
    target_position_.x += x_offset;
    target_position_.y += y_offset;
    alignBoundaries();
}

// TODO: combine zoomIn and zoomIn (x,y) methods later
void Camera::zoomIn()
{
    if (zoom_level_ >= MAX_ZOOM_LEVEL) return;
    zoom_level_+=0.5f;

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_;
    alignBoundaries();
}

void Camera::zoomIn(const float x, const float y)
{
    if (zoom_level_ >= MAX_ZOOM_LEVEL) return;
    zoomIn();

    auto half_of_target_size = target_size_/2.0f;

    const float local_x = x / zoom_level_;
    const float local_y = y / zoom_level_;

    const float new_position_x  = target_position_.x + (local_x - half_of_target_size.x);
    const float new_position_y  = target_position_.y + (local_y - half_of_target_size.y);
    setPosition(new_position_x, new_position_y);
}


void Camera::zoomOut()
{
    if (zoom_level_ > MIN_ZOOM_LEVEL) zoom_level_-=0.5f;
    else {return;}  

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_;
    alignBoundaries();
}

const sf::Vector2f& Camera::getPosition()
{
    return current_position_;
}

const sf::Vector2f& Camera::getSize()
{
    return current_size_;
}

void Camera::update()
{
    const auto position_diff = target_position_ - current_position_;
    current_position_ += position_diff  * MOVE_FACTOR;

    const auto zoom_diff = target_size_ - current_size_;
    current_size_ += zoom_diff  * ZOOM_FACTOR;
}

float Camera::getZoom()
{
    return  original_size_.x / current_size_.x;
}

}  // namespace game
