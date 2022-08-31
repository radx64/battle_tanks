#ifndef MATH_HPP_
#define MATH_HPP

#include <SFML/System/Vector2.hpp>

namespace math
{
double distance(double x1, double y1, double x2, double y2);
double signed_fmod(double a, double n);
double degree_to_radians(double degree);
sf::Vector2f rotate_point(sf::Vector2f point_to_rotate, double angle, sf::Vector2f pivot);
}  // namespace math

#endif  // MATH_HPP