
#include <SFML/System/Vector2.hpp>

#include "engine/Vector2.hpp"

namespace engine::temporary
{

template <typename T>
sf::Vector2<T> toSf(const engine::Vector2<T>& vector)
{
    return sf::Vector2<T>(static_cast<T>(vector.x), static_cast<T>(vector.y));
}

}  // namespace engine::temporary
