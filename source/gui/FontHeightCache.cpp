#include "gui/FontHeightCache.hpp"

#include <unordered_map>
#include <utility>

#include <fmt/format.h>
#include <SFML/Graphics.hpp>

#include "engine/Logger.hpp"

namespace 
{
template <class T1, class T2>
struct PairHash 
{
    std::size_t operator() (const std::pair<T1, T2>& pair) const
    {
        return std::hash<T1>()(pair.first) ^ std::hash<T2>()(pair.second);
    }
};
}

float getFontHeight(const sf::Font& font, unsigned int characterSize)
{
    using HashFunc = PairHash<const sf::Font*, unsigned int>;
    static std::unordered_map<std::pair<const sf::Font*, unsigned int>, float, HashFunc> cache{};

    const auto key = std::make_pair(&font, characterSize);
    const auto it = cache.find(key);
    if (it != cache.end())
    {
        return it->second;
    }

    engine::Logger logger{"FontHeightCache"};

    logger.debug(fmt::format("Calculating font height for font: {}, size: {}", fmt::ptr(&font), characterSize));
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setString("Ay|_");
    const auto height = text.getLocalBounds().height;
    logger.debug(fmt::format("Calculated height: {}", height));

    cache[key] = height;

    return height;
}
