
#include <SFML/Graphics.hpp>

constexpr float MOVE_FACTOR = 0.2;
constexpr float ZOOM_FACTOR = 0.2;

class Camera
{
public:
    Camera(const sf::Vector2f& position, const sf::Vector2f& size)
    : current_position_(position)
    , target_position_(position)
    , current_size_(size)
    , target_size_(size)
    {}

    void move(const float x_offset, const float y_offset)
    {
        target_position_.x += x_offset;
        target_position_.y += y_offset;
    }

    void zoom_in()
    {
        target_size_.x -= current_size_.x * ZOOM_FACTOR;
        target_size_.y -= current_size_.y * ZOOM_FACTOR;   
    }

    void zoom_out()
    {
        target_size_.x += current_size_.x * ZOOM_FACTOR;
        target_size_.y += current_size_.y * ZOOM_FACTOR;
    }

    const sf::Vector2f& get_position()
    {
        return current_position_;
    }

    const sf::Vector2f& get_size()
    {
        return current_size_;
    }

    void physics()
    {
        const auto position_diff = target_position_ - current_position_;
        current_position_ += position_diff  * MOVE_FACTOR;

        const auto zoom_diff = target_size_ - current_size_;
        current_size_ += zoom_diff  * MOVE_FACTOR;
    }

protected:
    sf::Vector2f current_position_;
    sf::Vector2f target_position_;
    sf::Vector2f current_size_;
    sf::Vector2f target_size_;
};