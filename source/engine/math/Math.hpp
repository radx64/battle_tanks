#pragma once

#include <cstddef>
#include <numeric>
#include <vector>

#include <SFML/System/Vector2.hpp>

namespace engine::math
{
float distance(float x1, float y1, float x2, float y2);
float dotProduct(float x1, float y1, float x2, float y2);
float crossProduct(float x1, float y1, float x2, float y2);
float normalizeVector(sf::Vector2f& vector);
float signedFmod(float a, float n);
float degreeToRadians(float degree);
sf::Vector2f rotatePoint(sf::Vector2f pointToRotate, float angle, sf::Vector2f pivot);
sf::Vector2f lerp(const sf::Vector2f& a, const sf::Vector2f& b, float t);
float lerp(const float a, const float b, float t);

class Average
{
public:
    Average (const size_t windowSize);
    int calculate(int newValue);

protected:
    size_t windowSize_{};
    std::vector<int> measurements_{};
    size_t currentIndex_{};
};

}  // namespace engine::math
