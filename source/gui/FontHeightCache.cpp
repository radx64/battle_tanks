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

namespace gui {

namespace
{
sf::FloatRect getReferenceTextBounds(const sf::Font& font, unsigned int characterSize)
{
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(characterSize);
    text.setString("Ay|_,q");
    return text.getLocalBounds();
}
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
    const auto height = getReferenceTextBounds(font, characterSize).height;
    logger.debug(fmt::format("Calculated height: {}", height));

    cache[key] = height;

    return height;
}

float getFontTop(const sf::Font& font, unsigned int characterSize)
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
    logger.debug(fmt::format("Calculating font top for font: {}, size: {}", fmt::ptr(&font), characterSize));
    const auto top = getReferenceTextBounds(font, characterSize).top;
    logger.debug(fmt::format("Calculated top: {}", top));

    cache[key] = top;

    return top;
}

float getFontLineSpacing(const sf::Font& font, unsigned int characterSize)
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
    logger.debug(fmt::format("Calculating line spacing for font: {}, size: {}", fmt::ptr(&font), characterSize));
    float lineSpacing = font.getLineSpacing(characterSize);
    logger.debug(fmt::format("Calculated line spacing: {}", lineSpacing));

    cache[key] = lineSpacing;

    return lineSpacing;
}

}  // namespace gui
