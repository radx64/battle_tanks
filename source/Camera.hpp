#include <SFML/Graphics.hpp>

class Camera
{
public:
    Camera(const sf::Vector2f& position, const sf::Vector2f& size);
    void set_position(const float x, const float y);
    void move(const float x_offset, const float y_offset);
    void zoom_in();
    void zoom_in(const float x, const float y);
    void zoom_out();
    const sf::Vector2f& get_position();
    const sf::Vector2f& get_size();
    void physics();

protected:
    void align_boundaries();
    sf::Vector2f current_position_;
    sf::Vector2f target_position_;
    const sf::Vector2f original_size_;
    sf::Vector2f current_size_;
    sf::Vector2f target_size_;
    float zoom_level_;
};