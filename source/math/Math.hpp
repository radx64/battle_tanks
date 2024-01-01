#ifndef MATH_MATH_HPP_
#define MATH_MATH_HPP_

#include <cstddef>
#include <numeric>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace math
{
float distance(float x1, float y1, float x2, float y2);
float dot_product(float x1, float y1, float x2, float y2);
float cross_product(float x1, float y1, float x2, float y2);
float normalize_vector(sf::Vector2f& vector);
float signed_fmod(float a, float n);
float degree_to_radians(float degree);
sf::Vector2f rotate_point(sf::Vector2f point_to_rotate, float angle, sf::Vector2f pivot);

class Average
{
public:
    Average (const size_t window_size);
    int calculate(int new_value);

protected:
    size_t window_size_{};
    std::vector<int> measurements_{};
    size_t current_index_{};
};

}  // namespace math

#endif  // MATH_MATH_HPP_