#include "Camera.hpp"

constexpr float MOVE_FACTOR = 0.1;
constexpr float ZOOM_FACTOR = 0.1;

constexpr float MIN_ZOOM_LEVEL = 1;
constexpr float MAX_ZOOM_LEVEL = 4;

Camera::Camera(const sf::Vector2f& position, const sf::Vector2f& size)
: current_position_{position}
, target_position_{position}
, original_size_{size}
, current_size_{size}
, target_size_{size}
, zoom_level_{MIN_ZOOM_LEVEL}
{}

void Camera::move(const float x_offset, const float y_offset)
{
    target_position_.x += x_offset;
    target_position_.y += y_offset;
}

void Camera::zoom_in()
{
    if (zoom_level_ < MAX_ZOOM_LEVEL) zoom_level_++;
    else {return;}

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_; 
}

void Camera::zoom_out()
{
    if (zoom_level_ > MIN_ZOOM_LEVEL) zoom_level_--;
    else {return;}  

    target_size_.x = original_size_.x / zoom_level_;
    target_size_.y = original_size_.y / zoom_level_;
}

const sf::Vector2f& Camera::get_position()
{
    return current_position_;
}

const sf::Vector2f& Camera::get_size()
{
    return current_size_;
}

void Camera::physics()
{
    const auto position_diff = target_position_ - current_position_;
    current_position_ += position_diff  * MOVE_FACTOR;

    const auto zoom_diff = target_size_ - current_size_;
    current_size_ += zoom_diff  * ZOOM_FACTOR;
}